////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2010-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "LICENSE.txt"
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "videoPYLON.h"
#include "plugins/PluginFactory.h"

#include <sstream>
using namespace gem::plugins;

#include "Gem/RTE.h"
#include "Gem/Exception.h"

#if 0
# define debug ::post
#else
# include "Utils/nop.h"
# define debug nop_post
#endif

class IEH : public Pylon::CImageEventHandler
{
  videoPYLON*m_parent;
public:
  IEH(videoPYLON*parent)
    : CImageEventHandler()
    , m_parent(parent)
  {  }
  virtual void OnImageGrabbed( Pylon::CInstantCamera& camera, const Pylon::CGrabResultPtr& ptrGrabResult)
    {
MARK();
        std::cout << "IEH::OnImageGrabbed called." << std::endl;
        std::cout << "IEH::MaxNumBuffer=" << camera.MaxNumBuffer()
                  << " MaxNumQueuedBuffer=" << camera.MaxNumQueuedBuffer()
                  << " MaxNumGrabResults=" << camera.MaxNumGrabResults()
                  << std::endl;
        std::cout << "IEH::NumReadyBuffers=" << camera.NumReadyBuffers()
                  << " NumQueuedBuffers=" << camera.NumQueuedBuffers()
                  << " NumEmptyBuffers=" << camera.NumEmptyBuffers()
                  << std::endl;


        if(ptrGrabResult->GrabSucceeded()) {
          std::cout << "SizeX: " << ptrGrabResult->GetWidth() << std::endl;
          std::cout << "SizeY: " << ptrGrabResult->GetHeight() << std::endl;
          const uint8_t *pImageBuffer = (uint8_t *) ptrGrabResult->GetBuffer();
          std::cout << "Gray value of first pixel: " << (uint32_t) pImageBuffer[0] << std::endl << std::endl;

        } else
          std::cerr << "OOPS[" << ptrGrabResult->GetErrorCode() << "] " << ptrGrabResult->GetErrorDescription() << std::endl;
        std::cout << std::endl;

        camera.ExecuteSoftwareTrigger();

    }
};


/////////////////////////////////////////////////////////
//
// videoPYLON
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

REGISTER_VIDEOFACTORY("pylon", videoPYLON);

videoPYLON :: videoPYLON()
  : m_factory(0)

  , m_name(std::string("pylon"))
  , m_devicename(std::string(""))
  , m_devicenum(0)
  , m_width(0), m_height(0)
  , m_async(true), m_grabloop(Pylon::GrabLoop_ProvidedByInstantCamera)

{
  MARK();

  m_camera.RegisterConfiguration( new Pylon::CSoftwareTriggerConfiguration, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_Delete);
  m_camera.RegisterImageEventHandler( new IEH(this), Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_Delete);

  m_provides.push_back(m_name);

  try {
    m_factory = &Pylon::CTlFactory::GetInstance();
    Pylon::TlInfoList_t tli;
    if (m_factory->EnumerateTls (tli))
      for (auto tl = tli.begin(); tl != tli.end(); ++tl) {
        std::string S=tl->GetFriendlyName().c_str();
        std::string s=S;
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        if (std::find(m_provides.begin(), m_provides.end(), s) == m_provides.end()) {
          // not in m_provides, add it
          m_provides.push_back(S);
          if(s != S)
            m_provides.push_back(s);
        }
      }
  } catch (GenICam::GenericException &e) {
    // Error handling
    throw(GemException(e.GetDescription()));
  }
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
videoPYLON :: ~videoPYLON()
{
MARK();
  close();
}

const std::string videoPYLON::getName(void)
{
MARK();
  return m_name;
}
bool videoPYLON::provides(const std::string&name)
{
MARK();
  for(unsigned int i=0; i<m_provides.size(); i++)
    if(name == m_provides[i]) {
      return true;
    }
  return false;
}
std::vector<std::string>videoPYLON::provides(void)
{
MARK();
  return m_provides;
}


bool videoPYLON::enumProperties(gem::Properties&readable,
                                gem::Properties&writeable)
{
MARK();
  gem::Properties props;
  std::vector<std::string>keys;
  gem::any type;

  readable.clear();
  writeable.clear();

#warning enumProps
#if 0
  props=gem::pylon::streamgrabberproperties::getKeys();
  keys=props.keys();

  props=gem::pylon::streamgrabberproperties::setKeys();
  keys=props.keys();

  props=gem::pylon::cameraproperties::getKeys();
  keys=props.keys();
  if(m_camera) {
    GenApi::INodeMap*nodes=m_camera.GetNodeMap();

    for(unsigned int i=0; i<keys.size(); i++) {
      GenApi::INode *node=nodes->GetNode(keys[i].c_str());
      if(node) {
        switch(node->GetAccessMode()) {
        case GenApi::RO:
        case GenApi::RW:
          readable.set(keys[i], props.get(keys[i]));
        default:
          break;
        }
      }
    }
  }

  props=gem::pylon::cameraproperties::setKeys();
  keys=props.keys();

  if(m_camera) {
    GenApi::INodeMap*nodes=m_camera.GetNodeMap();

    for(unsigned int i=0; i<keys.size(); i++) {
      GenApi::INode *node=nodes->GetNode(keys[i].c_str());
      if(node) {
        switch(node->GetAccessMode()) {
        case GenApi::WO:
        case GenApi::RW:
          writeable.set(keys[i], props.get(keys[i]));
        default:
          break;
        }
      }
    }
  }

#if 0
    try {
      const Pylon::CDeviceInfo & di=m_camera.GetDeviceInfo();
      Pylon::StringList_t names;
      di.GetPropertyNames (names);

      int i=0;
      for(unsigned int i=0; i<names.size(); i++) {
        std::string key=names[i].c_str();
        //        std::cerr << "property#"<<i<<": "<<names[i]<<std::endl;
        writeable.set(key, type);
        readable.set (key, type);
      }
    } catch (GenICam::GenericException &e) {
      error("[GEM:videoPYLON] %s", e.GetDescription());
      return false;
    }
#endif

#endif

  return false;
}
void videoPYLON::setProperties(gem::Properties&props)
{
MARK();
#warning setProps
#if 0
  std::vector<std::string>keys=props.keys();
  for(unsigned int i=0; i<keys.size(); i++) {
    const std::string key=keys[i];
    bool didit=false;

    if(!didit && m_camera)
      try {
        didit=gem::pylon::cameraproperties::set(m_camera, key, props);
      } catch (GenICam::GenericException &e) {
        verbose(0, "[GEM:videoPYLON] [%s] %s", key.c_str(), e.GetDescription());
        didit=false;
      }

  }
#endif
}

void videoPYLON::getProperties(gem::Properties&props)
{
MARK();
#warning getProps
#if 0
  std::vector<std::string>keys=props.keys();
  for(unsigned int i=0; i<keys.size(); i++) {
    const std::string key=keys[i];
    props.erase(key);
    gem::any result;

    if(result.empty() && m_camera)
      try {
        gem::pylon::cameraproperties::get(m_camera, key, result);
      } catch (GenICam::GenericException &e) {
        result.reset();
      }

    if(result.empty()) {
      continue;
    }
    props.set(key, result);
  }
#endif
}

std::vector<std::string> videoPYLON::enumerate()
{
MARK();
  m_id2device.clear();
  std::vector<std::string> result;
  if(!m_factory) {
    return result;
  }

  Pylon::DeviceInfoList_t devices;
  if (0 == m_factory->EnumerateDevices(devices))  {
    verbose(1, "[GEM:videoPYLON] could not enumerate");
    return result;
  }
  if(devices.empty() ) {
    return result;
  }

  int i=0;
  for(i=0; i<devices.size(); i++) {
    std::string name;
    Pylon::CDeviceInfo&device=devices[i];
    bool added=false;
#if 1
    //#define SHOWNAME(x) std::cerr << x<<"["<<i<<"]: "<<name<<std::endl;
#define SHOWNAME(x) verbose(0, "%d[%s]\t%s", i, x, name.c_str())
#else
#define SHOWNAME(x)
#endif

    name=device.GetUserDefinedName();
    if(!name.empty()) {
      m_id2device[name]=device;
      SHOWNAME("user");
    }
    if(!added && !name.empty()) {
      result.push_back(name);
      added=true;
    }

#warning TODO: get GigE-cams by IP

    name=device.GetFullName();
    if(!name.empty()) {
      m_id2device[name]=device;
      SHOWNAME("full");
    }
    if(!added && !name.empty()) {
      result.push_back(name);
      added=true;
    }

    name=device.GetSerialNumber();
    if(!name.empty()) {
      m_id2device[name]=device;
      SHOWNAME("serial");
    }
    if(!added && !name.empty()) {
      result.push_back(name);
      added=true;
    }

    name=device.GetFriendlyName();
    if(!name.empty()) {
      m_id2device[name]=device;
      SHOWNAME("friendly");
    }
    if(!added && !name.empty()) {
      result.push_back(name);
      added=true;
    }
  }

  return result;
}

bool videoPYLON::setDevice(int ID)
{
MARK();
  m_devicename.clear();
  m_devicenum=ID;
  return true;
}
bool videoPYLON::setDevice(const std::string&device)
{
MARK();
  m_devicename=device;
  return true;
}


/////////////////////////////////////////////////////////
// openDevice
//
/////////////////////////////////////////////////////////

bool videoPYLON :: open(gem::Properties&props)
{
MARK();
  double d;
  uint32_t channel=0;
  if(props.get("channel", d)) {
    channel=d;
  }

  if(!m_factory) {
    return false;
  }

  Pylon::IPylonDevice *device = 0;
  post("open '%s' or %d", m_devicename.c_str(), m_devicenum);

  try {
    if(m_devicename.empty()) {
      if(m_id2device.empty()) {
        enumerate();
      }
      std::map<std::string, Pylon::CDeviceInfo>::iterator it=m_id2device.begin();
      if(m_devicenum>=m_id2device.size())
        return false;
      if(m_devicenum>=0) {
        std::advance( it, m_devicenum );
      }
      if(it != m_id2device.end()) {
        post("creating device '%s'", it->first.c_str());
        device = m_factory->CreateDevice(it->second);
      }
    } else {
      std::map<std::string, Pylon::CDeviceInfo>::iterator it=m_id2device.find(
            m_devicename);
      if(it!=m_id2device.end()) {
        post("Creating device '%s'", it->first.c_str());
        device = m_factory->CreateDevice(it->second);
      } else {
        post("Creating device '%s'", m_devicename.c_str());
        device = m_factory->CreateDevice(Pylon::String_t(m_devicename.c_str()));
      }
    }
  } catch (GenICam::GenericException &e) {
    verbose(0, "[GEM:videoPYLON] %s", e.GetDescription());
    return false;
  }

  if(device) {
    verbose(0, "[GEM:videoPYLON] found device '%s'", device->GetDeviceInfo().GetFullName().c_str());
  } else {
    verbose(1, "[GEM:videoPYLON] creating device failed!");
    return false;
  }

  try {
    m_camera.Attach(device);
    m_camera.Open();
  } catch (GenICam::GenericException &e) {
    verbose(0, "[GEM:videoPYLON] %s", e.GetDescription());
    close();
    return false;
  } catch (std::exception &e) {
    verbose(0, "[GEM:videoPYLON] %s", e.what());
    close();
    return false;
  }
  return true;
}
/////////////////////////////////////////////////////////
// closeDevice
//
/////////////////////////////////////////////////////////
void videoPYLON :: close()
{
MARK();
/* attaching NULL will free the old device */
  m_camera.Attach(0);
}


/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
bool videoPYLON :: start()
{
MARK();
#if 0
  if(!m_camera.IsPylonDeviceAttached()
     || m_camera.IsCameraDeviceRemoved()) {
    return false;
  }
#endif

  if (!m_camera.CanWaitForFrameTriggerReady()) {
    error("[GEM:videoPYLON] camera cannot be queried whether it's ready to accept the next frame trigger");
  }

  try {
    Pylon::EGrabStrategy grabstrategy = Pylon::GrabStrategy_LatestImageOnly;
    //grabstrategy = Pylon::GrabLoop_ProvidedByInstantCamera;
    m_grabloop = m_async?(Pylon::GrabLoop_ProvidedByInstantCamera):(Pylon::GrabLoop_ProvidedByUser);
    m_camera.StartGrabbing(grabstrategy, m_grabloop);
  } catch (GenICam::GenericException &e) {
    error("[GEM:videoPYLON] %s", e.GetDescription());
    return false;
  }

  return true;
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
bool videoPYLON :: stop()
{
MARK();
  // Stop acquisition
  try {
    m_camera.StopGrabbing();
  } catch (GenICam::GenericException &e) {
    error("[GEM:videoPYLON] %s", e.GetDescription());
  }
  return true;
}

bool videoPYLON :: reset()
{
MARK();
#warning TODO reset
  return false;
}

////////////////////////////////////////////////////////
// frame grabber
//
/////////////////////////////////////////////////////////
pixBlock*videoPYLON::getFrame(void)
{
  MARK();

  m_pixBlock.image.setCsizeByFormat(GL_RGBA);
  m_pixBlock.image.xsize = m_width;
  m_pixBlock.image.ysize = m_height;
  m_pixBlock.image.reallocate();
  m_pixBlock.newimage = true;

  if(m_grabloop == Pylon::GrabLoop_ProvidedByUser) {
    bool didit;
    Pylon::CGrabResultPtr grabResult;
    post("manual grabbing");
    try {
      didit = m_camera.RetrieveResult(0, grabResult, Pylon::TimeoutHandling_Return);
    } catch (GenICam::GenericException &e) {
      error("grabbing: %s", e.GetDescription());
    }

    if(didit && grabResult->GrabSucceeded())
      post("grabbing done");
    else      post("grabbing done");

      post("grabbing failed");
  } else {
    post("trigger");
    m_camera.ExecuteSoftwareTrigger();
  }

  return &m_pixBlock;
}
void videoPYLON::releaseFrame(void)
{
MARK();
  m_pixBlock.newimage=false;
  //m_mutex.unlock();
}


bool videoPYLON::grabAsynchronous(bool async) {
  MARK();
  bool ret = m_async;
  m_async = async;
  return ret;
}
bool videoPYLON::setColor(int) {
  MARK();
  return false;
}
