////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2014 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#if 0
# define MARK() printf("%s:%d\t%s\n", __FILE__, __LINE__, __FUNCTION__)
#else
# define MARK()
#endif

#include <stdio.h>

#include "videoVIDS.h"
#include "plugins/PluginFactory.h"

#include "Gem/Exception.h"
#include "Gem/RTE.h"

#define MAXVIDSSTRING 1024

using namespace gem::plugins;

REGISTER_VIDEOFACTORY("VIDS", videoVIDS);

namespace
{
/* static instance for enumerating,... */
videoInput s_videoInput;
};

videoVIDS::videoVIDS(void)
  : m_name(std::string("videoInput"))
  , m_vi(0)
  , m_ID(-1), m_wantID(-1)
  , m_width(64), m_height(64)
{
  videoInput::setVerbose(false);
  MARK();
}

videoVIDS::~videoVIDS(void)
{
  close();
}
const std::string videoVIDS::getName(void)
{
  return m_name;
}
bool videoVIDS::provides(const std::string&name)
{
  return (name==m_name);
}
std::vector<std::string>videoVIDS::provides(void)
{
  std::vector<std::string>result;
  result.push_back(m_name);
  return result;
}

std::vector<std::string>videoVIDS::enumerate(void)
{
  videoInput*vi=(m_vi!=0)?m_vi:&s_videoInput;
  vi->listDevices();
  std::vector<std::string>result=vi->getDeviceList();
  return result;
}

bool videoVIDS::setDevice(int ID)
{
  m_wantID=ID;
  return true;
}
bool videoVIDS::setDevice(const std::string&device)
{
  m_wantID=-2;
  videoInput*vi=(m_vi!=0)?m_vi:&s_videoInput;
  if(vi->listDevices(true)) {
    std::vector<std::string>devs=vi->getDeviceList();
    for(unsigned int i=0; i<devs.size(); i++) {
      if(devs[i]==device) {
        m_wantID=i;
        return true;
      }
    }
  }
  return false;
}

namespace
{
static inline  void setVideoFilter(videoInput&vi, int device,
                                   const std::string&propName, const long propValue,
                                   const gem::Properties&props)
{
  double d;
  if(props.get(propName, d)) {
    vi.setVideoSettingFilterPct(device, propValue, d);
  }
}
static inline  void setVideoCamera(videoInput&vi, int device,
                                   const std::string&propName, const long propValue,
                                   const gem::Properties&props)
{
  double d;
  if(props.get(propName, d)) {
    vi.setVideoSettingCameraPct(device, propValue, d);
  }
}
static inline void getVideoFilter(videoInput&vi, int device,
                                  const std::string&propName, const long propValue,
                                  gem::Properties&props)
{
  long min=0;
  long max=0;
  long step=0;
  long cur=0;
  long flags=0;
  long def=0;

  props.erase(propName);

  if(!vi.getVideoSettingFilter(device, propValue, min, max, step, cur, flags,
                               def)) {
    return;
  }
  if(max==min) {
    return;
  }
  double d=((double)(cur-min))/((double)(max-min));
  verbose(1, "[GEM::videoVIDS] gotFilterSetting '%s' to %f (%d in %d..%d)",
          propName.c_str(), d, (int)cur, (int)min, (int)max);
  props.set(propName, d);
}
static inline void getVideoCamera(videoInput&vi, int device,
                                  const std::string&propName, const long propValue,
                                  gem::Properties&props)
{
  long min=0;
  long max=0;
  long step=0;
  long cur=0;
  long flags=0;
  long def=0;

  props.erase(propName);

  if(!vi.getVideoSettingCamera(device, propValue, min, max, step, cur, flags,
                               def)) {
    return;
  }
  if(max==min) {
    return;
  }
  double d=((double)(cur-min))/((double)(max-min));
  verbose(1, "[GEM::videoVIDS] gotCameraSetting '%s' to %f (%d in %d..%d)\n",
          propName.c_str(), d, (int)cur, (int)min, (int)max);
  props.set(propName, d);
}
};
#define VIDS_SET_FILTER(propName, propValue) \
  if(propName==key && m_vi)setVideoFilter(*m_vi, m_ID, propName, propValue, props)
#define VIDS_SET_CAMERA(propName, propValue) \
  if(propName==key && m_vi)setVideoCamera(*m_vi, m_ID, propName, propValue, props)
#define VIDS_GET_FILTER(propName, propValue) \
  if(propName==key && m_vi)getVideoFilter(*m_vi, m_ID, propName, propValue, props)
#define VIDS_GET_CAMERA(propName, propValue) \
  if(propName==key && m_vi)getVideoCamera(*m_vi, m_ID, propName, propValue, props)

bool videoVIDS::enumProperties(gem::Properties&readable,
                               gem::Properties&writeable)
{
  readable.clear();
  writeable.clear();

  writeable.set("width",  m_pixBlock.image.xsize);
  readable.set("width",  m_pixBlock.image.xsize);
  writeable.set("height",  m_pixBlock.image.ysize);
  readable.set("height",  m_pixBlock.image.ysize);

  writeable.set("fps",  30);

  int id=(m_ID<0)?m_wantID:m_ID;
  videoInput*vi=(m_vi!=0)?m_vi:&s_videoInput;
#define ENUM_READWRITE(type, key, value)                         \
    getVideo##type(*vi, id, key, vi->value, readable);        \
    getVideo##type(*vi, id, key, vi->value, writeable);       \

  if(id>=0) {
    ENUM_READWRITE(Filter, "brightness", propBrightness);
    ENUM_READWRITE(Filter, "contrast", propContrast);
    ENUM_READWRITE(Filter, "hue", propHue);
    ENUM_READWRITE(Filter, "saturation", propSaturation);
    ENUM_READWRITE(Filter, "sharpness", propSharpness);
    ENUM_READWRITE(Filter, "gamma", propGamma);
    ENUM_READWRITE(Filter, "colorenable", propColorEnable);
    ENUM_READWRITE(Filter, "whitebalance", propWhiteBalance);
    ENUM_READWRITE(Filter, "backlightcompensation", propBacklightCompensation);

    ENUM_READWRITE(Camera, "gain", propGain);
    ENUM_READWRITE(Camera, "pan", propPan);
    ENUM_READWRITE(Camera, "tilt", propTilt);
    ENUM_READWRITE(Camera, "roll", propRoll);
    ENUM_READWRITE(Camera, "zoom", propZoom);
    ENUM_READWRITE(Camera, "exposure", propExposure);
    ENUM_READWRITE(Camera, "iris", propIris);
    ENUM_READWRITE(Camera, "focus", propFocus);
  }

  writeable.set("verbose",  0);

  return true;
}

void videoVIDS::setProperties(gem::Properties&props)
{
  if(trySetProperties(props, true)) {
    verbose(1, "[GEM::videoVIDS] needs restart");
    if(m_vi) {
      stop();
      start();
    }
  }
}
/* returns 'true' if we need to restart */
bool videoVIDS::trySetProperties(gem::Properties&props, bool canrestart)
{
  m_props=props;
  double dval;
  if(props.get("verbose", dval)) {
    int i = (int)dval;
    videoInput::setVerbose(i>0);
  }
  if(!m_vi) {
    return false;
  }
  std::vector<std::string>keys=props.keys();
  bool needrestart = false;
  if(canrestart && props.type("width") != gem::Properties::UNSET) {
    if((m_props.get("width", dval)) && (int)dval!=m_width) {
      needrestart = true;
    }
  }
  if(props.type("height") != gem::Properties::UNSET) {
    if((m_props.get("height", dval)) && (int)dval!=m_height) {
      needrestart = true;
    }
  }
  if(canrestart && needrestart) {
    return true;
  }
  for(unsigned i=0; i<keys.size(); i++) {
    std::string key=keys[i];
    std::string sval;
    if("verbose" == key) {
    } else if(("fps" == key) || ("framerate" == key) ) {
      if(props.get(key, dval)) {
        if(m_vi) {
          m_vi->setIdealFramerate(m_ID, dval);
        }
      }
    } else if(("norm" == key) && m_vi) {
      if(props.get(key, sval)) {
        int format=-1;
        if (0) {
          format=-1;
        } else if ("NTSC"     == sval) {
          format=VI_NTSC_M;  /* default NTSC */
        } else if ("NTSC_M"   == sval) {
          format=VI_NTSC_M;
        } else if ("PAL"      == sval) {
          format=VI_PAL_B;  /* default PAL */
        } else if ("PAL_B"    == sval) {
          format=VI_PAL_B;
        } else if ("PAL_D"    == sval) {
          format=VI_PAL_D;
        } else if ("PAL_G"    == sval) {
          format=VI_PAL_G;
        } else if ("PAL_H"    == sval) {
          format=VI_PAL_H;
        } else if ("PAL_I"    == sval) {
          format=VI_PAL_I;
        } else if ("PAL_M"    == sval) {
          format=VI_PAL_M;
        } else if ("PAL_N"    == sval) {
          format=VI_PAL_N;
        } else if ("PAL_NC"   == sval) {
          format=VI_PAL_NC;
        } else if ("SECAM"    == sval) {
          format=VI_SECAM_B;  /* default SECAM */
        } else if ("SECAM_B"  == sval) {
          format=VI_SECAM_B;
        } else if ("SECAM_D"  == sval) {
          format=VI_SECAM_D;
        } else if ("SECAM_G"  == sval) {
          format=VI_SECAM_G;
        } else if ("SECAM_H"  == sval) {
          format=VI_SECAM_H;
        } else if ("SECAM_K"  == sval) {
          format=VI_SECAM_K;
        } else if ("SECAM_K1" == sval) {
          format=VI_SECAM_K1;
        } else if ("SECAM_L"  == sval) {
          format=VI_SECAM_L;
        } else if ("NTSC_M_J" == sval) {
          format=VI_NTSC_M_J;
        } else if ("NTSC_433" == sval) {
          format=VI_NTSC_433;
        }
        if(format>=0) {
          m_vi->setFormat(m_ID, format);
        }
      }
    }
    // *INDENT-OFF*
    else VIDS_SET_FILTER("brightness", m_vi->propBrightness);
    else VIDS_SET_FILTER("contrast", m_vi->propContrast);
    else VIDS_SET_FILTER("hue", m_vi->propHue);
    else VIDS_SET_FILTER("saturation", m_vi->propSaturation);
    else VIDS_SET_FILTER("sharpness", m_vi->propSharpness);
    else VIDS_SET_FILTER("gamma", m_vi->propGamma);
    else VIDS_SET_FILTER("colorenable", m_vi->propColorEnable);
    else VIDS_SET_FILTER("whitebalance", m_vi->propWhiteBalance);
    else VIDS_SET_FILTER("backlightcompensation", m_vi->propBacklightCompensation);
    else VIDS_SET_FILTER("gain", m_vi->propGain);

    else VIDS_SET_CAMERA("pan", m_vi->propPan);
    else VIDS_SET_CAMERA("tilt", m_vi->propTilt);
    else VIDS_SET_CAMERA("roll", m_vi->propRoll);
    else VIDS_SET_CAMERA("zoom", m_vi->propZoom);
    else VIDS_SET_CAMERA("exposure", m_vi->propExposure);
    else VIDS_SET_CAMERA("iris", m_vi->propIris);
    else VIDS_SET_CAMERA("focus", m_vi->propFocus);
    // *INDENT-ON*
  }
  return needrestart;
}

void videoVIDS::getProperties(gem::Properties&props)
{
  if(!m_vi) {
    return;
  }

  std::vector<std::string>keys=props.keys();

  for(int i=0; i<keys.size(); i++) {
    double d;
    std::string key=keys[i];
    if("width"==key) {
      props.set(key, m_pixBlock.image.xsize);
    } else if("height"==key) {
      props.set(key, m_pixBlock.image.ysize);
    }
    // *INDENT-OFF*
    else VIDS_GET_FILTER("brightness", m_vi->propBrightness);
    else VIDS_GET_FILTER("contrast", m_vi->propContrast);
    else VIDS_GET_FILTER("hue", m_vi->propHue);
    else VIDS_GET_FILTER("saturation", m_vi->propSaturation);
    else VIDS_GET_FILTER("sharpness", m_vi->propSharpness);
    else VIDS_GET_FILTER("gamma", m_vi->propGamma);
    else VIDS_GET_FILTER("colorenable", m_vi->propColorEnable);
    else VIDS_GET_FILTER("whitebalance", m_vi->propWhiteBalance);
    else VIDS_GET_FILTER("backlightcompensation", m_vi->propBacklightCompensation);
    else VIDS_GET_FILTER("gain", m_vi->propGain);

    else VIDS_GET_CAMERA("pan", m_vi->propPan);
    else VIDS_GET_CAMERA("tilt", m_vi->propTilt);
    else VIDS_GET_CAMERA("roll", m_vi->propRoll);
    else VIDS_GET_CAMERA("zoom", m_vi->propZoom);
    else VIDS_GET_CAMERA("exposure", m_vi->propExposure);
    else VIDS_GET_CAMERA("iris", m_vi->propIris);
    else VIDS_GET_CAMERA("focus", m_vi->propFocus);
    // *INDENT-ON*
  }
}

bool videoVIDS::open(gem::Properties&props)
{
  MARK();
  m_props=props;
  const int numdevs = s_videoInput.listDevices(true);
  if(-1 == m_wantID && 0 < numdevs) {
    m_wantID = 0;
  }
  if(m_wantID>=0 && m_wantID<numdevs) {
    return true;
  }

  MARK();
  return false;
}
void videoVIDS::close(void)
{
  MARK();
  stop();
  MARK();
}

bool videoVIDS::start(void)
{
  MARK();
  if(m_wantID>=0) {
    if(m_vi) {
      delete m_vi;
    }
    m_vi=new videoInput();
    MARK();
    m_width=m_vi->getWidth(m_wantID);
    MARK();
    m_height=m_vi->getHeight(m_wantID);
    MARK();

    double d;
    MARK();
    if((m_props.get("width", d)) && d>=1.) {
      m_width=d;
    }
    MARK();
    if((m_props.get("height", d)) && d>=1.) {
      m_height=d;
    }
    MARK();
    if(m_vi->setupDevice(m_wantID, m_width, m_height)) {
      MARK();
      m_width=m_vi->getWidth(m_wantID);
      m_height=m_vi->getHeight(m_wantID);
      MARK();
      m_ID=m_wantID;
      trySetProperties(m_props, false);
      return true;
    }
    MARK();
  }
  MARK();
  if(m_vi) {
    delete m_vi;
  }
  m_vi=0;
  return false;
}
bool videoVIDS::stop (void)
{
  bool success = false;
  MARK();
  if(m_vi) {
    m_vi->stopDevice(m_ID);
    delete m_vi;
    m_vi = 0;
    success = true;
  }
  MARK();
  m_ID=-1;
  return success;
}
bool videoVIDS::reset()
{
  if(m_vi) {
    return m_vi->restartDevice(m_ID);
  }
  return false;
}

pixBlock*videoVIDS::getFrame(void)
{
  if(!m_vi) {
    return 0;
  }
  if(m_vi->isFrameNew(m_ID)) {
    m_width=m_vi->getWidth(m_ID);
    m_height=m_vi->getHeight(m_ID);
    unsigned char*data=m_pixBlock.image.data;
    m_pixBlock.image.xsize=m_width;
    m_pixBlock.image.ysize=m_height;
    m_pixBlock.image.setFormat(GEM_RGBA);
    m_pixBlock.image.reallocate();
    m_pixBlock.newimage=(data==m_pixBlock.image.data);

    m_pixBlock.image.fromRGB(m_vi->getPixels(m_ID, true, true));
    m_pixBlock.newimage=true;
  } else {
    m_pixBlock.newimage=false;
  }
  return &m_pixBlock;
}

void videoVIDS::releaseFrame(void)
{
}

std::vector<std::string>videoVIDS::dialogs(void)
{
  std::vector<std::string>result;
  result.push_back("format");
  return result;
}
bool videoVIDS::dialog(std::vector<std::string>dlgs)
{
  if(!m_vi) {
    return false;
  }

  bool doit=false;

  if(dlgs.empty()) {
    doit=true;
  }

  if(!doit) {
    for(unsigned int i=0; i<dlgs.size(); i++)
      if(dlgs[i]=="format") {
        doit=true;
      }
  }
  if(doit) {
    m_vi->showSettingsWindow(m_ID);
  }
  return doit;
}

bool videoVIDS::isThreadable()
{
  return false;
}
bool videoVIDS::grabAsynchronous(bool)
{
  return false;
}
bool videoVIDS::setColor(int)
{
  return false;
}
