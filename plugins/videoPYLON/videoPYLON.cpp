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

class gem::plugins::videoPYLON::ImageEventHandler : public Pylon::CImageEventHandler
{
public:
  videoPYLON*m_parent;
  Pylon::CImageFormatConverter m_converter;
  Pylon::CLock m_colorlock, m_pixlock;
  pixBlock m_pix;
  GLenum m_color;
  ImageEventHandler(videoPYLON*parent)
    : CImageEventHandler()
    , m_parent(parent)
    , m_color(GL_RGBA)
  {
    m_converter.OutputBitAlignment = Pylon::OutputBitAlignment_LsbAligned;
    setFormat(m_color);
  }
  virtual void OnImageGrabbed( Pylon::CInstantCamera& camera, const Pylon::CGrabResultPtr& ptrGrabResult)
    {
        if(ptrGrabResult->GrabSucceeded()) {
          // convert
          Pylon::AutoLock pixlock(m_pixlock);
          m_pix.newimage = true;
          m_pix.image.xsize = ptrGrabResult->GetWidth();
          m_pix.image.ysize = ptrGrabResult->GetHeight();
          m_pix.image.setCsizeByFormat(m_color);
          m_pix.image.reallocate();
          m_converter.Convert (m_pix.image.data, m_pix.image.xsize*m_pix.image.ysize*m_pix.image.csize, ptrGrabResult);
        } else
          std::cerr << "OOPS[" << ptrGrabResult->GetErrorCode() << "] " << ptrGrabResult->GetErrorDescription() << std::endl;

        //camera.ExecuteSoftwareTrigger();

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
  , m_ieh(new ImageEventHandler(this))

  , m_name(std::string("pylon"))
  , m_devicename(std::string(""))
  , m_devicenum(0)
  , m_width(0), m_height(0)
  , m_async(true), m_grabloop(Pylon::GrabLoop_ProvidedByInstantCamera)

{
  MARK();
  m_camera.RegisterConfiguration( new Pylon::CSoftwareTriggerConfiguration, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_Delete);
  m_camera.RegisterImageEventHandler( m_ieh, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_Delete);

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

namespace {
  double any2float(const gem::any&value) {
    try {
      return gem::any_cast<float>(value);
    } catch (gem::bad_any_cast&e) {;}
    try {
      double d = gem::any_cast<double>(value);
      return d;
    } catch (gem::bad_any_cast&e) {throw(e);}
    return 0.;
  }
  int any2int(const gem::any&value) {
    try {
      return gem::any_cast<int>(value);
    } catch (gem::bad_any_cast&e) {;}
    return any2float(value);
  }
  gem::any node2any(GenApi::INode*node) {
    gem::any result;
    GenApi::EInterfaceType interfacetype = node->GetPrincipalInterfaceType();
    try {
      switch(interfacetype) {
      case GenApi::intfIBoolean:
        result = (double)GenApi::CBooleanPtr(node)->GetValue();
        break;
      case GenApi::intfIInteger:
        result = (double)GenApi::CIntegerPtr(node)->GetValue();
        break;
      case GenApi::intfIFloat:
        result = (double)(GenApi::CFloatPtr(node)->GetValue());
        break;
      case GenApi::intfIString:
        result = std::string(GenApi::CStringPtr(node)->GetValue().c_str());
        break;
      case GenApi::intfIEnumeration:
        result =
#if 1
          std::string(GenApi::CEnumerationPtr(node)->GetCurrentEntry()->GetSymbolic().c_str());
#else
        (double)(GenApi::CEnumerationPtr(node)->GetIntValue());
#endif
        break;
      default:    // don't show interfaces we cannot use directly
        break;
      }
    } catch (GenICam::GenericException &e) {
      verbose(0, "[GEM:videoPYLON] %s", e.GetDescription());
      result.reset();
    }
    return result;
  }
  bool any2node(GenApi::INode*node, const gem::any&value) {
    GenApi::EInterfaceType interfacetype = node->GetPrincipalInterfaceType();
    try {
      double d;
      std::string s;
      switch(interfacetype) {
      case GenApi::intfIBoolean:
        try {
          GenApi::CBooleanPtr(node)->SetValue(any2int(value));
        } catch (gem::bad_any_cast&e) { return false; }
        break;
      case GenApi::intfIInteger:
        try {
          GenApi::CIntegerPtr(node)->SetValue(any2int(value));
        } catch (gem::bad_any_cast&e) { return false; }
        break;
      case GenApi::intfIFloat:
        try {
          GenApi::CFloatPtr(node)->SetValue(any2float(value));
        } catch (gem::bad_any_cast&e) { return false; }
        break;
      case GenApi::intfIString:
        try {
          GenApi::CStringPtr(node)->SetValue(gem::any_cast<std::string>(value).c_str());
        } catch (gem::bad_any_cast&e) { return false; }
        break;
      case GenApi::intfICommand:
        if(!value.empty())
          error("[GEM::videoPYLON] ignoring argument to command");
        GenApi::CCommandPtr(node)->Execute();
        break;
      case GenApi::intfIEnumeration: {
        bool didit = false;
        GenApi::CEnumerationPtr enumptr(node);
        if(!didit)
          try {
            enumptr->FromString(gem::any_cast<std::string>(value).c_str());
            didit = true;
          } catch (gem::bad_any_cast&e) { didit=false; }
        if(!didit)
          try {
            enumptr->SetIntValue(any2int(value));
            didit = true;
          } catch (gem::bad_any_cast&e) { didit=false; }
        if(!didit) return false;
        break;
      }
      default:    // ignore interfaces we cannot use directly
        break;
      }
    } catch (GenICam::GenericException &e) {
      verbose(0, "[GEM:videoPYLON] %s", e.GetDescription());
      return false;
    }
    return true;
  }
  void nodemap2properties(GenApi::INodeMap&nodemap,
                          gem::Properties&readable,
                          gem::Properties&writeable) {
    GenApi::NodeList_t nodes;
    nodemap.GetNodes(nodes);
    for(uint64_t nodecount = 0; nodecount < nodes.size(); nodecount++) {
      auto node = nodes[nodecount];
      if(node->GetVisibility()  == GenApi::Invisible) continue;
      // it seems that only features are interesting...
      if(!node->IsFeature())continue;

      gem::any result = node2any(node);
      if(result.empty() && (GenApi::intfICommand != node->GetPrincipalInterfaceType())) {
        continue;
      }
      Pylon::String_t name(node->GetName());
      switch(node->GetAccessMode()) {
      case GenApi::RO:
        readable.set(name.c_str(), result);
        break;
      case GenApi::WO:
        writeable.set(name.c_str(), result);
        break;
      case GenApi::RW:
        readable.set(name.c_str(), result);
        writeable.set(name.c_str(), result);
        break;
      default: break;
      }
    }
  }
};
bool videoPYLON::enumProperties(gem::Properties&readable,
                                gem::Properties&writeable)
{
  gem::Properties props;
  std::vector<std::string>keys;
  gem::any type;

  readable.clear();
  writeable.clear();

  nodemap2properties(m_camera.GetStreamGrabberNodeMap(), readable, writeable);
  //nodemap2properties(m_camera.GetTLNodeMap(), readable, writeable);
  //nodemap2properties(m_camera.GetEventGrabberNodeMap(), readable, writeable);
  //nodemap2properties(m_camera.GetInstantCameraNodeMap(), readable, writeable);

  nodemap2properties(m_camera.GetNodeMap(), readable, writeable);
  return true;
}
void videoPYLON::setProperties(gem::Properties&props)
{
  GenApi::INodeMap&camnodes = m_camera.GetNodeMap();
  auto&streamnodes = m_camera.GetStreamGrabberNodeMap();
  std::vector<std::string>keys=props.keys();
  for(auto keyptr = keys.begin(); keyptr != keys.end(); ++keyptr) {
    std::string key = *keyptr;

    if(false);
    else if("width" == key) key = "Width";
    else if("height" == key) key = "Height";
    else if("leftmargin" == key) key = "OffsetX";
    else if("topmargin" == key) key = "OffsetY";

    auto node = camnodes.GetNode(key.c_str());
    if(!node) node = streamnodes.GetNode(key.c_str());
    if(node) {
      any2node(node, props.get(key));
      if(node->GetPrincipalInterfaceType() == GenApi::intfICommand)
        props.erase(*keyptr);
    }
  }
}

void videoPYLON::getProperties(gem::Properties&props)
{
  GenApi::INodeMap&camnodes = m_camera.GetNodeMap();
  auto&streamnodes = m_camera.GetStreamGrabberNodeMap();

  std::vector<std::string>keys=props.keys();
  for(auto keyptr = keys.begin(); keyptr != keys.end(); ++keyptr) {
    std::string key = *keyptr;
    gem::any result;
    props.erase(*keyptr);

    if(false);
    else if("width" == key) key = "Width";
    else if("height" == key) key = "Height";
    else if("leftmargin" == key) key = "OffsetX";
    else if("topmargin" == key) key = "OffsetY";

    auto node = camnodes.GetNode(key.c_str());
    if(!node) node = streamnodes.GetNode(key.c_str());

    if(node) {
      result = node2any(node);
    }
    if(result.empty()) continue;
    props.set(*keyptr, result);
  }
}

std::vector<std::string> videoPYLON::enumerate()
{
MARK();
  std::vector<std::string> result;
  if(!m_factory) {
    return result;
  }

  Pylon::DeviceInfoList_t devices;
  if (!m_factory->EnumerateDevices(devices))  {
    verbose(1, "[GEM:videoPYLON] could not enumerate %d", devices.size());
    return result;
  }

  for(auto device = devices.begin(); device != devices.end(); ++device) {
    std::string name;
    bool added=false;
#if 0
#define SHOWNAME(x, s) if(!s.empty())verbose(1, "[GEM::videoPYLON] device#%d['%s']\t%s", device - devices.begin(), x, s.c_str())
#else
#define SHOWNAME(x, s)
#endif
#define ADDNAME(dev, prop) do { Pylon::String_t name;         \
      if (dev->GetPropertyValue(prop, name)) {           \
        SHOWNAME(prop, name);                                    \
        if(!added && !name.empty()) {                            \
          result.push_back(name.c_str());                        \
          added = true;                                          \
        } } } while(0);

    ADDNAME(device, "UserDefinedName");
    ADDNAME(device, "FullName");
    ADDNAME(device, "SerialNumber");
    ADDNAME(device, "FriendlyName");
    ADDNAME(device, "IpAddress");
    ADDNAME(device, "MacAddress");
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

  try {
    if(m_devicename.empty()) {
      Pylon::DeviceInfoList_t devices;
      if(m_factory->EnumerateDevices(devices) > m_devicenum ) {
        device = m_factory->CreateDevice(devices[m_devicenum]);
      }
    } else {
      const char* devname = m_devicename.c_str();
#define ADDFILTER(filt, prop, value) \
        filt.push_back(Pylon::CDeviceInfo().SetPropertyValue(prop, value));

      Pylon::DeviceInfoList_t devices, filter;
      ADDFILTER(filter, "UserDefinedName", devname);
      ADDFILTER(filter, "FullName", devname);
      ADDFILTER(filter, "FriendlyName", devname);
      ADDFILTER(filter, "SerialNumber", devname);
      ADDFILTER(filter, "IpAddress", devname);
      ADDFILTER(filter, "MacAddress", devname);

      if(m_factory->EnumerateDevices(devices, filter)) {
        for(auto dev = devices.begin(); dev != devices.end(); ++dev) {
          device = m_factory->CreateDevice(*dev);
          if(device)
            break;
        }
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
  setProperties(props);
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
    //post("trigger");
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
