////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2010 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "LICENSE.txt"
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_LIBPYLONUTILITY
//# undef HAVE_LIBPYLONUTILITY
#endif


#include "videoPYLON.h"
#include <sstream>
using namespace gem;

#define NUM_BUFFERS 8

#include "Gem/RTE.h"
#include "Base/GemException.h"

#if 0
# define debug ::post
#else
# define debug
#endif

using namespace Basler_GigECameraParams;
using namespace Basler_GigEStreamParams;


#include "CameraProperties.h"
#include "StreamGrabberProperties.h"

// Constructor allocates the image buffer

class videoPYLON::CGrabBuffer {
  static int buffercount;
public:

  CGrabBuffer(const size_t ImageSize)
    : m_pBuffer(NULL)  {
    buffercount++;
    m_pBuffer = new uint8_t[ ImageSize ];
    if (NULL == m_pBuffer)
      {
        GenICam::GenericException e("Not enough memory to allocate image buffer", __FILE__, __LINE__);
        throw e;
      }
  }

  // Freeing the memory
  ~CGrabBuffer() {
    if (NULL != m_pBuffer)
      delete[] m_pBuffer;
    buffercount--;
  }
  uint8_t* GetBufferPointer(void) { return m_pBuffer; }
  Pylon::StreamBufferHandle GetBufferHandle(void) { return m_hBuffer; }
  void SetBufferHandle(Pylon::StreamBufferHandle hBuffer) { m_hBuffer = hBuffer; };
  
protected:
  uint8_t *m_pBuffer;
  Pylon::StreamBufferHandle m_hBuffer;

};


static unsigned int pixelformat2pitch (enum Basler_GigECameraParams::PixelFormatEnums format) {
  switch(format) {
  default: return 0;
  case Basler_GigECameraParams::PixelFormat_Mono8: return 1;
  case Basler_GigECameraParams::PixelFormat_Mono8Signed: return 1;
  case Basler_GigECameraParams::PixelFormat_Mono10: return 0;
  case Basler_GigECameraParams::PixelFormat_Mono12: return 0;
  case Basler_GigECameraParams::PixelFormat_Mono16: return 0;
  case Basler_GigECameraParams::PixelFormat_Mono16Signed: return 2;
  case Basler_GigECameraParams::PixelFormat_BayerGR8: return 1;
  case Basler_GigECameraParams::PixelFormat_BayerRG8: return 1;
  case Basler_GigECameraParams::PixelFormat_BayerGB8: return 1;
  case Basler_GigECameraParams::PixelFormat_BayerBG8: return 1;
  case Basler_GigECameraParams::PixelFormat_BayerGR10: return 0;
  case Basler_GigECameraParams::PixelFormat_BayerRG10: return 0;
  case Basler_GigECameraParams::PixelFormat_BayerGB10: return 0;
  case Basler_GigECameraParams::PixelFormat_BayerBG10: return 0;
  case Basler_GigECameraParams::PixelFormat_BayerGR12: return 0;
  case Basler_GigECameraParams::PixelFormat_BayerRG12: return 0;
  case Basler_GigECameraParams::PixelFormat_BayerGB12: return 0;
  case Basler_GigECameraParams::PixelFormat_BayerBG12: return 0;
  case Basler_GigECameraParams::PixelFormat_BayerGR16: return 2;
  case Basler_GigECameraParams::PixelFormat_BayerRG16: return 2;
  case Basler_GigECameraParams::PixelFormat_BayerGB16: return 2;
  case Basler_GigECameraParams::PixelFormat_BayerBG16: return 2;
  case Basler_GigECameraParams::PixelFormat_RGB8Planar: return 1;
  case Basler_GigECameraParams::PixelFormat_RGB10Planar: return 0;
  case Basler_GigECameraParams::PixelFormat_RGB12Planar: return 0;
  case Basler_GigECameraParams::PixelFormat_RGB16Planar: return 2;
  
  }
  return 0;
}


int videoPYLON::CGrabBuffer::buffercount=0;
/////////////////////////////////////////////////////////
//
// videoPYLON
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
#ifdef HAVE_PYLON

REGISTER_VIDEOFACTORY("pylon", videoPYLON);

videoPYLON :: videoPYLON() : video("pylon")
                           , m_factory(NULL)
                           , m_camera(NULL)
                           , m_grabber(NULL)
#ifdef HAVE_LIBPYLONUTILITY
                           , m_converter(NULL)
#endif
                           , m_numBuffers(NUM_BUFFERS)
{
  m_width=0;
  m_height=0;

  try {
    m_factory = &Pylon::CTlFactory::GetInstance();
    Pylon::TlInfoList_t tli;
    int count= m_factory->EnumerateTls (tli);
    int i=0;
    for(i=0; i<count; i++) {
      std::string s=tli[i].GetFriendlyName().c_str();
      if("GigE"==s)
        provide("gige");
      provide(s);
    }

  } catch (GenICam::GenericException &e) {
    // Error handling
    throw(GemException(e.GetDescription()));
    return;
  }
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
videoPYLON :: ~videoPYLON()
{
  close();
}

////////////////////////////////////////////////////////
// frame grabber
//
/////////////////////////////////////////////////////////
bool videoPYLON :: grabFrame() {
  if(NULL==m_camera || NULL==m_grabber)
    return false;

  struct Pylon::SImageFormat imageFormat;
  struct Pylon::SOutputImageFormat outImageFormat;

  if(m_grabber->GetWaitObject().Wait(3000)) {
    Pylon::GrabResult Result;
    m_grabber->RetrieveResult(Result);
    switch(Result.Status()) {
    case Pylon::Grabbed: {
      unsigned int pitch = Pylon::IsPacked(Result.GetPixelType())?0:
        (7+Result.GetSizeX()*Pylon::PixelSize(Result.GetPixelType())) >>3;

      

      lock();
      m_image.image.xsize=Result.GetSizeX();
      m_image.image.ysize=Result.GetSizeY();
      //      m_image.image.setCsizeByFormat(GL_RGBA);
      m_image.image.reallocate();
#ifdef HAVE_LIBPYLONUTILITY
      if(m_converter) {
        imageFormat.Width =Result.GetSizeX();
        imageFormat.Height=Result.GetSizeY();
        imageFormat.LinePitch=pitch;
        imageFormat.PixelFormat=Result.GetPixelType();

        if(GL_RGBA==m_image.image.format) {
          outImageFormat.LinePitch=Result.GetSizeX()*m_image.image.csize;
          outImageFormat.PixelFormat=Pylon::PixelType_RGBA8packed;
        } else {
          outImageFormat.LinePitch=Result.GetSizeX()*m_image.image.csize;
          outImageFormat.PixelFormat=Pylon::PixelType_Mono8;
        }

        if(!m_converter->IsInitialized()) {
          std::cerr << "initializing" << std::endl;
          m_converter->Init(imageFormat);
          std::cerr << "initialized" << std::endl;
        }
          
        m_converter->Convert(m_image.image.data,
                             m_image.image.xsize*m_image.image.ysize*m_image.image.csize,
                             Result.Buffer(),
                             Result.GetPayloadSize(),
                             imageFormat,
                             outImageFormat);
      } else 
#endif
      m_image.image.fromGray((unsigned char*)Result.Buffer());

      m_image.image.upsidedown=true;
      m_image.newimage=true;
      unlock();
      m_grabber->QueueBuffer(Result.Handle(), NULL);

    }
      break;
    case Pylon::Failed:
      std::cerr << "PYLON: grab failed and ";
    default:
      std::cerr << "PYLON: grab returned "<<Result.Status()<<std::endl;
      break;
    }
  } else {
    // timeout
    std::cerr << "PYLON: grab timed out"<<std::endl;
    m_grabber->CancelGrab();
    return false;
  }

  return true;
}


/////////////////////////////////////////////////////////
// openDevice
//
/////////////////////////////////////////////////////////

bool videoPYLON :: openDevice(gem::Properties&props)
{
  double d;
  uint32_t channel=0;
  if(props.get("channel", d))
    channel=d;

  if(m_camera)closeDevice();
  if(NULL==m_factory)return false;

  Pylon::IPylonDevice *device = NULL;
  try {
    std::map<std::string, Pylon::CDeviceInfo>::iterator it=m_id2device.find(m_devicename);
    if(it!=m_id2device.end())
      device = m_factory->CreateDevice(it->second);
    else
      device = m_factory->CreateDevice(Pylon::String_t(m_devicename.c_str()));
  } catch (GenICam::GenericException &e) {
    std::cerr << e.GetDescription() << std::endl;
    return false;
  }

  if(device==NULL)
    return false;

  try {
    m_camera=new Pylon::CBaslerGigECamera::CBaslerGigECamera (device);
    m_camera->Open();
    uint32_t maxchannel=m_camera->GetNumStreamGrabberChannels();
    if(channel>maxchannel)channel=maxchannel;

    m_grabber=new Pylon::CBaslerGigEStreamGrabber(m_camera->GetStreamGrabber(channel));
  } catch (GenICam::GenericException &e) {
    std::cerr << e.GetDescription() << std::endl;
    close();
    return false;
  }

  return true;
}
/////////////////////////////////////////////////////////
// closeDevice
//
/////////////////////////////////////////////////////////
void videoPYLON :: closeDevice() {
  if(m_camera){
    m_camera->Close();
    delete m_camera;
  }
  m_camera=NULL;

  if(m_grabber)delete m_grabber;
  m_grabber=NULL;
}


/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
bool videoPYLON :: startTransfer()
{
  if(NULL==m_camera)
    return false;

  if(NULL==m_grabber)
    return false;

  try {
    m_grabber->Open();

    // Set the image format and AOI
    //    m_camera->PixelFormat.SetValue(Basler_GigECameraParams::PixelFormat_Mono8Signed);
    //    m_camera->OffsetX.SetValue(0);
    //    m_camera->OffsetY.SetValue(0);

    //    m_camera->Width.SetValue(m_camera->Width.GetMax());
    //    m_camera->Height.SetValue(m_camera->Height.GetMax());

    // Set the camera to continuous frame mode
    /*
    m_camera->TriggerSelector.SetValue(TriggerSelector_AcquisitionStart);
    m_camera->TriggerMode.SetValue(TriggerMode_Off);
    m_camera->AcquisitionMode.SetValue(AcquisitionMode_Continuous);
    */
    //    m_camera->ExposureMode.SetValue(ExposureMode_Timed);
    //    m_camera->ExposureTimeRaw.SetValue(100);

    const size_t ImageSize = (size_t)(m_camera->PayloadSize.GetValue());
    m_grabber->MaxBufferSize.SetValue(ImageSize);
    m_grabber->MaxNumBuffer.SetValue(m_numBuffers);
    m_grabber->PrepareGrab();

    uint32_t i;
    for (i = 0; i < m_numBuffers; ++i) {
      CGrabBuffer *pGrabBuffer = new CGrabBuffer(ImageSize);
      pGrabBuffer->SetBufferHandle(m_grabber->RegisterBuffer(
                                                             pGrabBuffer->GetBufferPointer(),
                                                             ImageSize));

      // Put the grab buffer object into the buffer list
      m_buffers.push_back(pGrabBuffer);
    }
    std::vector<CGrabBuffer*>::const_iterator x;
    for (x=m_buffers.begin(); x != m_buffers.end(); ++x) {
      // Put buffer into the grab queue for grabbing
      m_grabber->QueueBuffer((*x)->GetBufferHandle(), NULL);
    }

    m_camera->AcquisitionStart.Execute();
  } catch (GenICam::GenericException &e) {
    std::cerr << e.GetDescription() << std::endl;
    return false;
  }

#ifdef HAVE_LIBPYLONUTILITY
  bool rgba_out=false;
  bool need_converter=true;

  if(m_converter) {
    delete m_converter;
    m_converter=NULL;
  }
 
  switch(m_camera->PixelFormat.GetValue()) {
  case Basler_GigECameraParams::PixelFormat_Mono8:
  case Basler_GigECameraParams::PixelFormat_Mono8Signed:
    /* no converter needed */
    need_converter=false;
    rgba_out=false;
    break;

  case Basler_GigECameraParams::PixelFormat_RGBA8Packed:
    /* no converter needed */
    need_converter=false;
    rgba_out=true;
    break;

  case Basler_GigECameraParams::PixelFormat_Mono10:
  case Basler_GigECameraParams::PixelFormat_Mono12:
  case Basler_GigECameraParams::PixelFormat_Mono16:
  case Basler_GigECameraParams::PixelFormat_Mono16Signed:
    m_converter= new	Pylon::CPixelFormatConverterGamma ();
    // m_converter= new	Pylon::CPixelFormatConverterTruncate ();
    rgba_out=false;
    break;

  case Basler_GigECameraParams::PixelFormat_Mono10Packed:
  case Basler_GigECameraParams::PixelFormat_Mono12Packed:
    m_converter= new	Pylon::CPixelFormatConverterGammaPacked ();
    // m_converter= new	Pylon::CPixelFormatConverterTruncatePacked ();
    rgba_out=false;
    break;

  case Basler_GigECameraParams::PixelFormat_BayerGR8:
  case Basler_GigECameraParams::PixelFormat_BayerRG8:
  case Basler_GigECameraParams::PixelFormat_BayerGB8:
  case Basler_GigECameraParams::PixelFormat_BayerBG8:
  case Basler_GigECameraParams::PixelFormat_BayerGR10:
  case Basler_GigECameraParams::PixelFormat_BayerRG10:
  case Basler_GigECameraParams::PixelFormat_BayerGB10:
  case Basler_GigECameraParams::PixelFormat_BayerBG10:
  case Basler_GigECameraParams::PixelFormat_BayerGR12:
  case Basler_GigECameraParams::PixelFormat_BayerRG12:
  case Basler_GigECameraParams::PixelFormat_BayerGB12:
  case Basler_GigECameraParams::PixelFormat_BayerBG12:
  case Basler_GigECameraParams::PixelFormat_BayerGB12Packed:
  case Basler_GigECameraParams::PixelFormat_BayerGR12Packed:
  case Basler_GigECameraParams::PixelFormat_BayerRG12Packed:
  case Basler_GigECameraParams::PixelFormat_BayerBG12Packed:
  case Basler_GigECameraParams::PixelFormat_BayerGR16:
  case Basler_GigECameraParams::PixelFormat_BayerRG16:
  case Basler_GigECameraParams::PixelFormat_BayerGB16:
  case Basler_GigECameraParams::PixelFormat_BayerBG16:
    m_converter= new	Pylon::CPixelFormatConverterBayer ();
    rgba_out=true;
    break;

  case Basler_GigECameraParams::PixelFormat_RGB8Planar:
  case Basler_GigECameraParams::PixelFormat_RGB10Planar:
  case Basler_GigECameraParams::PixelFormat_RGB12Planar:
  case Basler_GigECameraParams::PixelFormat_RGB16Planar:

  case Basler_GigECameraParams::PixelFormat_RGB8Packed:
  case Basler_GigECameraParams::PixelFormat_BGR8Packed:
  case Basler_GigECameraParams::PixelFormat_BGRA8Packed:
  case Basler_GigECameraParams::PixelFormat_RGB10Packed:
  case Basler_GigECameraParams::PixelFormat_BGR10Packed:
  case Basler_GigECameraParams::PixelFormat_RGB12Packed:
  case Basler_GigECameraParams::PixelFormat_BGR12Packed:
  case Basler_GigECameraParams::PixelFormat_RGB10V1Packed:
  case Basler_GigECameraParams::PixelFormat_RGB10V2Packed:
  case Basler_GigECameraParams::PixelFormat_RGB12V1Packed:
    m_converter= new	Pylon::CPixelFormatConverterRGB ();
    rgba_out=true;
    break;


  case Basler_GigECameraParams::PixelFormat_YUV411Packed:
  case Basler_GigECameraParams::PixelFormat_YUV444Packed:
    /* no converter ? */
    break;

  case Basler_GigECameraParams::PixelFormat_YUV422Packed:
    m_converter= new	Pylon::CPixelFormatConverterYUV422();
    rgba_out=true;
    break;
  case Basler_GigECameraParams::PixelFormat_YUV422_YUYV_Packed:
    m_converter= new	Pylon::CPixelFormatConverterYUV422YUYV();
    rgba_out=true;
    break;

  }

  if(rgba_out) {
    m_image.image.setCsizeByFormat(GL_RGBA);
  } else {
    m_image.image.setCsizeByFormat(GL_LUMINANCE);
  }
  
  if(NULL==m_converter && need_converter) {
    error("PYLON: could not find a converter for given colorspace");
  }
#endif
  return true;
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
bool videoPYLON :: stopTransfer()
{
  if(m_camera) {
    // Stop acquisition
    try {
      m_camera->AcquisitionStop.Execute();
    } catch (GenICam::GenericException &e) {
      std::cerr << e.GetDescription() << std::endl;
    }
  }
  if(m_grabber) {
    try {
      m_grabber->CancelGrab();
      // Get all buffers back
      Pylon::GrabResult r;
      while(m_grabber->RetrieveResult(r)){;}

      std::vector<CGrabBuffer*>::iterator it;
      for (it = m_buffers.begin(); it != m_buffers.end(); it++) {
        m_grabber->DeregisterBuffer((*it)->GetBufferHandle());
        delete *it;
        *it = NULL;
      }
      m_buffers.clear();

      m_grabber->FinishGrab();
      m_grabber->Close();
    } catch (GenICam::GenericException &e) {
      std::cerr << e.GetDescription() << std::endl;
    }
  }
#ifdef HAVE_LIBPYLONUTILITY
  if(m_converter){
    delete m_converter;
    m_converter=NULL;
  }
#endif
  return true;
}

std::vector<std::string> videoPYLON::enumerate() {
  m_id2device.clear();
  std::vector<std::string> result;
  if(NULL==m_factory)return result;

  Pylon::DeviceInfoList_t devices;
  if (0 == m_factory->EnumerateDevices(devices))  {
    std::cout << "could not enumerate" << std::endl;
    return result;
  }
  if(devices.empty() )
    return result;

  int i=0;
  for(i=0; i<devices.size(); i++) {
    std::string name;
    Pylon::CDeviceInfo&device=devices[i];
    bool added=false;
#if 1
#define SHOWNAME(x) std::cerr << x<<"["<<i<<"]: "<<name<<std::endl;
#else
#define SHOWNAME(x)
#endif

    name=device.GetUserDefinedName();
    if(!name.empty()) { m_id2device[name]=device; SHOWNAME("user")}
    if(!added && !name.empty()) {  result.push_back(name);  added=true;  }

    try {
      /* darn, this doesn't seem to work..., it would be great though */
      Pylon::CBaslerGigEDeviceInfo&gdevice=dynamic_cast< Pylon::CBaslerGigEDeviceInfo&>(device);

      name=gdevice.GetAddress ();
      if(!name.empty()) { m_id2device[name]=device; SHOWNAME("addresse")}
      if(!added && !name.empty()) {  result.push_back(name);  added=true;  }

    } catch (const std::bad_cast& e) {
      //      std::cerr << i<<" not a GigE&device"<<std::endl;
    }

    name=device.GetFullName();
    if(!name.empty()) { m_id2device[name]=device; SHOWNAME("full")}
    if(!added && !name.empty()) {  result.push_back(name);  added=true;  }

    name=device.GetSerialNumber();
    if(!name.empty()) { m_id2device[name]=device; SHOWNAME("serial")}
    if(!added && !name.empty()) {  result.push_back(name);  added=true;  }

    name=device.GetFriendlyName();
    if(!name.empty()) { m_id2device[name]=device; SHOWNAME("friendly")}
    if(!added && !name.empty()) {  result.push_back(name);  added=true;  }
  }

  return result;
}


bool videoPYLON::enumProperties(gem::Properties&readable,
                                gem::Properties&writeable) {

  int i=0;
  gem::Properties props;
  std::vector<std::string>keys;
  gem::any type;

  readable.clear();
  writeable.clear();

  props=gem::pylon::streamgrabberproperties::getKeys();
  keys=props.keys();
  for(i=0; i<keys.size(); i++)
    readable.set(keys[i], type);

  props=gem::pylon::streamgrabberproperties::setKeys();
  keys=props.keys();
  for(i=0; i<keys.size(); i++)
    writeable.set(keys[i], type);


  props=gem::pylon::cameraproperties::getKeys();
  keys=props.keys();
  for(i=0; i<keys.size(); i++)
    readable.set(keys[i], type);

  props=gem::pylon::cameraproperties::setKeys();
  keys=props.keys();
  for(i=0; i<keys.size(); i++)
    writeable.set(keys[i], type);

  if(m_camera) {
    try {
      const Pylon::CDeviceInfo & di=m_camera->GetDeviceInfo();
      Pylon::StringList_t names;
      di.GetPropertyNames (names);

      int i=0;
      for(i=0; i<names.size(); i++) {
        std::string key=names[i].c_str();
        //        std::cerr << "property#"<<i<<": "<<names[i]<<std::endl;
        writeable.set(key, type);
        readable.set (key, type);
      }
    } catch (GenICam::GenericException &e) {
      std::cerr << e.GetDescription() << std::endl;
      return false;
    }
  }

  return false;
}
void videoPYLON::setProperties(gem::Properties&props) {
  std::vector<std::string>keys=props.keys();
  int i=0;
  for(i=0; i<keys.size(); i++) {
    const std::string key=keys[i];
    bool didit=false;
    if(m_grabber) {
      try {
        didit=gem::pylon::streamgrabberproperties::set(m_grabber, key, props);
      } catch (GenICam::GenericException &e) {
        error("videoPYLON: [%s] %s", key.c_str(), e.GetDescription());
        //std::cerr << e.GetDescription() << std::endl;
        didit=false;
      }
    }

    if(!didit && m_camera)
      try {
        didit=gem::pylon::cameraproperties::set(m_camera, key, props);
      } catch (GenICam::GenericException &e) {
        error("videoPYLON: [%s] %s", key.c_str(), e.GetDescription());
        //std::cerr << e.GetDescription() << std::endl;
        didit=false;
      }

    //    std::cerr << "setting "<<key<<" success: "<<didit<<std::endl;
  }
}

void videoPYLON::getProperties(gem::Properties&props) {
  std::vector<std::string>keys=props.keys();
  int i=0;
  for(i=0; i<keys.size(); i++) {
    const std::string key=keys[i];
    props.erase(key);
    gem::any result;
    if(m_grabber) {
      try {
        gem::pylon::streamgrabberproperties::get(m_grabber, key, result);
      } catch (GenICam::GenericException &e) {result.reset(); }
    }

    if(result.empty() && m_camera)
      try {
        gem::pylon::cameraproperties::get(m_camera, key, result);
      } catch (GenICam::GenericException &e) {result.reset(); }

    if(result.empty())
      continue;
    props.set(key, result);
  }
}


#else
videoPYLON :: videoPYLON() : video("")
{ }
videoPYLON :: ~videoPYLON()
{ }
#endif /* HAVE_PYLON */
