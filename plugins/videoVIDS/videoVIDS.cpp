////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
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

videoVIDS::videoVIDS(void) :
  m_name(std::string("videoInput")),
  m_ID(-1), m_wantID(-1),
  m_width(64), m_height(64)
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
  m_vi.listDevices();
  std::vector<std::string>result=m_vi.getDeviceList();
  return result;
}

bool videoVIDS::setDevice(int ID)
{
  m_wantID=ID;
  return true;
}
bool videoVIDS::setDevice(const std::string&device)
{
  if(m_vi.listDevices(true)) {
    std::vector<std::string>devs=m_vi.getDeviceList();
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
  verbose(1, "[GEM::videoVIDS] gotV '%s' to %f (%d in %d..%d)",
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
  verbose(1, "[GEM::videoVIDS] gotC '%s' to %f (%d in %d..%d)\n",
          propName.c_str(), d, (int)cur, (int)min, (int)max);
  props.set(propName, d);
}
};
#define VIDS_SET_FILTER(propName, propValue) \
  if(propName==key && m_ID>=0)setVideoFilter(m_vi, m_ID, propName, propValue, props)
#define VIDS_SET_CAMERA(propName, propValue) \
  if(propName==key && m_ID>=0)setVideoCamera(m_vi, m_ID, propName, propValue, props)
#define VIDS_GET_FILTER(propName, propValue) \
  if(propName==key && m_ID>=0)getVideoFilter(m_vi, m_ID, propName, propValue, props)
#define VIDS_GET_CAMERA(propName, propValue) \
  if(propName==key && m_ID>=0)getVideoCamera(m_vi, m_ID, propName, propValue, props)

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

  int id=m_ID;
  if(id<0) {
    id=m_wantID;
  }

#define ENUM_READWRITE(type, key, value)                        \
    getVideo##type(m_vi, id, key, m_vi.value, readable);        \
    getVideo##type(m_vi, id, key, m_vi.value, writeable);       \

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
  return true;
}

void videoVIDS::setProperties(gem::Properties&props)
{
  m_props=props;

  int width=-1;
  int height=-1;
  std::vector<std::string>keys=props.keys();
  for(unsigned i=0; i<keys.size(); i++) {
    std::string key=keys[i];
    double dval;
    std::string sval;

    if("width" == key) {
      if(props.get(key, dval)) {
        if(dval>0) {
          width = dval;
        }
      }
    } else if("height" == key) {
      if(props.get(key, dval)) {
        if(dval>0) {
          height = dval;
        }
      }
    } else if(("fps" == key) || ("framerate" == key) ) {
      if(props.get(key, dval)) {
        if(m_ID>=0) {
          m_vi.setIdealFramerate(m_ID, dval);
        }
      }
    } else if(("norm" == key) && m_ID>=0) {
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
          m_vi.setFormat(m_ID, format);
        }
      }
    } else {
      VIDS_SET_FILTER("brightness", m_vi.propBrightness);
    } else {
      VIDS_SET_FILTER("contrast", m_vi.propContrast);
    } else {
      VIDS_SET_FILTER("hue", m_vi.propHue);
    } else {
      VIDS_SET_FILTER("saturation", m_vi.propSaturation);
    } else {
      VIDS_SET_FILTER("sharpness", m_vi.propSharpness);
    } else {
      VIDS_SET_FILTER("gamma", m_vi.propGamma);
    } else {
      VIDS_SET_FILTER("colorenable", m_vi.propColorEnable);
    } else {
      VIDS_SET_FILTER("whitebalance", m_vi.propWhiteBalance);
    } else {
      VIDS_SET_FILTER("backlightcompensation", m_vi.propBacklightCompensation);
    } else {
      VIDS_SET_FILTER("gain", m_vi.propGain);
    }

    else {
      VIDS_SET_CAMERA("pan", m_vi.propPan);
    } else {
      VIDS_SET_CAMERA("tilt", m_vi.propTilt);
    } else {
      VIDS_SET_CAMERA("roll", m_vi.propRoll);
    } else {
      VIDS_SET_CAMERA("zoom", m_vi.propZoom);
    } else {
      VIDS_SET_CAMERA("exposure", m_vi.propExposure);
    } else {
      VIDS_SET_CAMERA("iris", m_vi.propIris);
    } else {
      VIDS_SET_CAMERA("focus", m_vi.propFocus);
    }
  }
  if(width>0 || height>0) {
    MARK();
    if(m_ID) {
      MARK();
      m_width=m_vi.getWidth(m_ID);
      MARK();
      m_height=m_vi.getHeight(m_ID);
      MARK();
    }
    MARK();
    if(width>0) {
      m_width=width;
    }
    if(height>0) {
      m_height=width;
    }
  }
}

void videoVIDS::getProperties(gem::Properties&props)
{
  std::vector<std::string>keys=props.keys();
  double d;
  int i;

  for(i=0; i<keys.size(); i++) {
    std::string key=keys[i];
    if("width"==key) {
      props.set(key, m_pixBlock.image.xsize);
    } else if("height"==key) {
      props.set(key, m_pixBlock.image.ysize);
    } else {
      VIDS_GET_FILTER("brightness", m_vi.propBrightness);
    } else {
      VIDS_GET_FILTER("contrast", m_vi.propContrast);
    } else {
      VIDS_GET_FILTER("hue", m_vi.propHue);
    } else {
      VIDS_GET_FILTER("saturation", m_vi.propSaturation);
    } else {
      VIDS_GET_FILTER("sharpness", m_vi.propSharpness);
    } else {
      VIDS_GET_FILTER("gamma", m_vi.propGamma);
    } else {
      VIDS_GET_FILTER("colorenable", m_vi.propColorEnable);
    } else {
      VIDS_GET_FILTER("whitebalance", m_vi.propWhiteBalance);
    } else {
      VIDS_GET_FILTER("backlightcompensation", m_vi.propBacklightCompensation);
    } else {
      VIDS_GET_FILTER("gain", m_vi.propGain);
    }

    else {
      VIDS_GET_CAMERA("pan", m_vi.propPan);
    } else {
      VIDS_GET_CAMERA("tilt", m_vi.propTilt);
    } else {
      VIDS_GET_CAMERA("roll", m_vi.propRoll);
    } else {
      VIDS_GET_CAMERA("zoom", m_vi.propZoom);
    } else {
      VIDS_GET_CAMERA("exposure", m_vi.propExposure);
    } else {
      VIDS_GET_CAMERA("iris", m_vi.propIris);
    } else {
      VIDS_GET_CAMERA("focus", m_vi.propFocus);
    }
  }
}

bool videoVIDS::open(gem::Properties&props)
{
  MARK();
  m_props=props;
  if(m_wantID>=0 && m_wantID<m_vi.listDevices(true)) {
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
  m_wantID=-1;
}

bool videoVIDS::start(void)
{
  MARK();
  if(m_wantID>=0) {
    MARK();
    m_width=m_vi.getWidth(m_wantID);
    MARK();
    m_height=m_vi.getHeight(m_wantID);
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
    if(m_vi.setupDevice(m_wantID, m_width, m_height)) {
      MARK();
      m_ID=m_wantID;
      return true;
    }
    MARK();
  }
  MARK();
  return false;
}
bool videoVIDS::stop (void)
{
  MARK();
  if(m_ID>=0) {
    MARK();
    m_vi.stopDevice(m_ID);
    MARK();
    m_ID=-1;
    return true;
  }
  MARK();
  return false;
}
bool videoVIDS::reset()
{
  if(m_ID>=0) {
    return m_vi.restartDevice(m_ID);
  }
  return false;
}

pixBlock*videoVIDS::getFrame(void)
{
  if(m_ID<0) {
    return 0;
  }
  if(m_vi.isFrameNew(m_ID)) {
    m_width=m_vi.getWidth(m_ID);
    m_height=m_vi.getHeight(m_ID);
    unsigned char*data=m_pixBlock.image.data;
    m_pixBlock.image.xsize=m_width;
    m_pixBlock.image.ysize=m_height;
    m_pixBlock.image.setCsizeByFormat(GL_RGBA);
    m_pixBlock.image.reallocate();
    m_pixBlock.newimage=(data==m_pixBlock.image.data);

    m_pixBlock.image.fromRGB(m_vi.getPixels(m_ID, true, true));
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
  bool doit=false;
  if(m_ID>=0) {
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
      m_vi.showSettingsWindow(m_ID);
    }
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
