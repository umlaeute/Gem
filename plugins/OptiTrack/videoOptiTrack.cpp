////////////////////////////////////////////////////////
//
// videoOptiTrack - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#ifdef HAVE_LIBOPTITRACK


/* when dynamically linking against camerlibrary, we have to set CAMERALIBRARY_IMPORTS
 *  this is a safeguard
 */
#ifndef CAMERALIBRARY_IMPORTS
# warning CAMERALIBRARY_IMPORTS
#endif

#include "videoOptiTrack.h"
#include "Gem/RTE.h"
#include "Gem/Exception.h"

#include "plugins/PluginFactory.h"
using namespace gem::plugins;

REGISTER_VIDEOFACTORY("OptiTrack", videoOptiTrack);

using namespace CameraLibrary;

namespace
{
struct CMInitBase {
  CMInitBase(void)
  {
    std::cerr << "OptiTrack: enable development...";
    cCameraLibraryStartupSettings::X().EnableDevelopment();
    std::cerr << "done"<<std::endl;
  }
};
struct CMInit : CMInitBase {
  CameraLibrary::CameraManager&m_camman;

  CMInit(void) :
    CMInitBase(),
    m_camman(CameraLibrary::CameraManager::X())
  {
    std::cerr << "OptiTrack: waiting for CameraManager init...";
    m_camman.WaitForInitialization();
    std::cerr << "done"<<std::endl;
  }
  ~CMInit(void)
  {
    std::cerr << "OptiTrack: shutting down CameraManager...";
    //CameraLibrary::CameraManager::X().Shutdown();
    std::cerr << "done"<<std::endl;
  }
};
static CMInit s_camlib;

static const std::string s_name = std::string("OptiTrack");
}

videoOptiTrack::videoOptiTrack(void) :
  m_devnum(-1),
  m_quality(-1),
  m_camera(NULL),
  m_frame(NULL),
  m_resize(true)
{
  m_pixBlock.image.xsize = 320;
  m_pixBlock.image.ysize = 240;
  m_pixBlock.image.setFormat(GEM_GRAY);
  m_pixBlock.image.allocate();
}

videoOptiTrack::~videoOptiTrack(void)
{
  close();
}
void videoOptiTrack::close(void)
{
  //MARK;
  stop();
  if(m_camera) {
    m_camera->Release();
  }
  m_camera=NULL;
}

bool videoOptiTrack::open(gem::Properties&props)
{
  if(!CameraManager::X().AreCamerasInitialized()) {
    return false;
  }
  CameraList list;
  if(!m_devname.empty()) {
    for(int i=0; i<list.Count(); i++) {
      if(list[i].Name()==m_devname) {
        m_camera = CameraManager::X().GetCamera(list[i].UID());
      }
      if(m_camera) {
        break;
      }
    }
    if(!m_camera) {
      return false;
    }
  } else if(m_devnum>=0) {
    if(m_devnum<list.Count()) {
      m_camera = CameraManager::X().GetCamera(list[m_devnum].UID());
    } else {
      return false;
    }
  }
  /* auto mode */
  if(!m_camera) {
    m_camera = CameraManager::X().GetCamera();
  }

  if(!m_camera) {
    return false;
  }
  m_pixBlock.image.xsize = m_camera->Width();
  m_pixBlock.image.ysize = m_camera->Height();

  m_camera->SetVideoType(MJPEGMode);
  m_quality=m_camera->MJPEGQuality();

  setProperties(props);
  return true;
}

pixBlock*videoOptiTrack::getFrame(void)
{
  if(!m_camera) {
    return NULL;
  }

  //m_frame = m_camera->GetFrame();
  m_frame = m_camera->GetLatestFrame();

  m_pixBlock.image.reallocate();
  if(m_resize) {
    m_pixBlock.image.setBlack();
  }
  m_resize=false;
  if(m_frame) {
    m_frame->Rasterize(m_pixBlock.image.xsize, m_pixBlock.image.ysize,
                       m_pixBlock.image.xsize, m_pixBlock.image.csize*8,
                       m_pixBlock.image.data);
    m_pixBlock.newimage=true;
    m_pixBlock.image.upsidedown=true;
  }
  return &m_pixBlock;
}

void videoOptiTrack::releaseFrame(void)
{
  if(m_frame) {
    m_frame->Release();
  }
  m_frame=NULL;
}

std::vector<std::string>videoOptiTrack::enumerate(void)
{
  std::vector<std::string>result;
  if(!CameraManager::X().AreCamerasInitialized()) {
    CameraManager::X().WaitForInitialization();
  }

  CameraList list;
  for(int i=0; i<list.Count(); i++) {
    result.push_back(list[i].Name());
  }

  return result;
}

bool videoOptiTrack::setDevice(int ID)
{
  m_devname.clear();
  m_devnum=ID;
  return true;
}
bool videoOptiTrack::setDevice(const std::string&device)
{
  m_devname=device;
  return true;
}


/*
  SetAEC(bool);
  SetAGC(bool);
  SetContinuousIR(bool);
  SetEnableBlockingMask(bool);
  SetHighPowerMode(bool);
  SetIRFilter(bool);
  SetMarkerOverlay(bool);
  SetTextOverlay(bool);

  SetExposure(int);
  SetFrameDecimation(int);
  SetFrameRate(int);
  SetGrayscaleDecimation(int);
  SetIntensity(int);
  SetPrecisionCap(int);
  SetShutterDelay(int);
  SetStatusIntensity(int);
  SetThreshold(int);

  SetName(const char*);


  SetVideoType(eVideoMode);
  SetLateMJPEGDecompression(bool);
  SetMJPEGQuality(int);

  SetAllLED(eStatusLEDs);
  SetLED(eStatusLEDs, bool);

  SetNumeric(bool, int);

  SetWindow(int, int, int, int);
  SetObjectColor(int);
  SetBitMaskPixel(int x, int y, bool);

  SetCameraParameter(char*name, float value);
*/

bool videoOptiTrack::enumProperties(gem::Properties&readable,
                                    gem::Properties&writeable)
{
  readable.clear();
  writeable.clear();

  writeable.set("width",  m_pixBlock.image.xsize);
  readable.set("width",  m_pixBlock.image.xsize);
  writeable.set("height",  m_pixBlock.image.ysize);
  readable.set("height",  m_pixBlock.image.ysize);

#define SETCAMERAPROP_BOOL(name) writeable.set(#name, 1);
#define SETCAMERAPROP_INT(name)  writeable.set(#name, 1);
#define SETCAMERAPROP_STR(name)  writeable.set(#name, std::string(""));
  SETCAMERAPROP_BOOL(AEC);
  SETCAMERAPROP_BOOL(AGC);
  SETCAMERAPROP_BOOL(ContinuousIR);
  SETCAMERAPROP_BOOL(EnableBlockingMask);
  SETCAMERAPROP_BOOL(HighPowerMode);
  SETCAMERAPROP_BOOL(IRFilter);
  SETCAMERAPROP_BOOL(MarkerOverlay);
  SETCAMERAPROP_BOOL(TextOverlay);
  SETCAMERAPROP_INT(Exposure);
  SETCAMERAPROP_INT(FrameDecimation);
  SETCAMERAPROP_INT(FrameRate);
  SETCAMERAPROP_INT(GrayscaleDecimation);
  SETCAMERAPROP_INT(Intensity);
  SETCAMERAPROP_INT(PrecisionCap);
  SETCAMERAPROP_INT(ShutterDelay);
  SETCAMERAPROP_INT(StatusIntensity);
  SETCAMERAPROP_INT(Threshold);
  SETCAMERAPROP_STR(Name);
#undef SETCAMERAPROP_BOOL
#undef SETCAMERAPROP_INT
#undef SETCAMERAPROP_STR

#define GETCAMERAPROP_BOOL(name) readable.set(#name, 1);
#define GETCAMERAPROP_INT(name)  readable.set(#name, 0);
#define GETCAMERAPROP_FLT(name)  readable.set(#name, 0);
#define GETCAMERAPROP_STR(name)  readable.set(#name, std::string(""));
  GETCAMERAPROP_BOOL(AEC);
  GETCAMERAPROP_BOOL(AGC);
  GETCAMERAPROP_BOOL(ContinuousIR);
  GETCAMERAPROP_BOOL(HighPowerMode);
  GETCAMERAPROP_BOOL(IRFilter);
  GETCAMERAPROP_BOOL(MarkerOverlay);
  GETCAMERAPROP_BOOL(TextOverlay);
  GETCAMERAPROP_INT(Exposure);
  GETCAMERAPROP_INT(FrameDecimation);
  GETCAMERAPROP_INT(FrameRate);
  GETCAMERAPROP_INT(GrayscaleDecimation);
  GETCAMERAPROP_INT(Intensity);
  GETCAMERAPROP_INT(PrecisionCap);
  GETCAMERAPROP_INT(ShutterDelay);
  GETCAMERAPROP_INT(Threshold);
  GETCAMERAPROP_FLT(fanspeed);
  GETCAMERAPROP_FLT(temperature);
  GETCAMERAPROP_STR(Name);
#undef GETCAMERAPROP_BOOL
#undef GETCAMERAPROP_INT
#undef GETCAMERAPROP_STR

  return true;
}
void videoOptiTrack::setProperties(gem::Properties&props)
{
  std::string s;
  m_props=props;


  double d;
  bool resize=false;
  if(props.get("width", d)) {
    if(d>0) {
      m_pixBlock.image.xsize=d;
      m_resize=true;
    }
  }
  if(props.get("height", d)) {
    if(d>0) {
      m_pixBlock.image.ysize=d;
      m_resize=true;
    }
  }

#define SETCAMERAPROP_BOOL(name) do { if(props.get(#name, d)) {bool b=(d>0.5); m_camera->Set##name(b); } } while(0)
#define SETCAMERAPROP_INT(name)  do { if(props.get(#name, d)) {int i=(int)d; m_camera->Set##name(i); } } while(0)
#define SETCAMERAPROP_STR(name)  do { if(props.get(#name, s)) {int i=(int)d; m_camera->Set##name(s.c_str()); } } while(0)
  SETCAMERAPROP_BOOL(AEC);
  SETCAMERAPROP_BOOL(AGC);
  SETCAMERAPROP_BOOL(ContinuousIR);
  SETCAMERAPROP_BOOL(EnableBlockingMask);
  SETCAMERAPROP_BOOL(HighPowerMode);
  SETCAMERAPROP_BOOL(IRFilter);
  SETCAMERAPROP_BOOL(MarkerOverlay);
  SETCAMERAPROP_BOOL(TextOverlay);
  SETCAMERAPROP_INT(Exposure);
  SETCAMERAPROP_INT(FrameDecimation);
  SETCAMERAPROP_INT(FrameRate);
  SETCAMERAPROP_INT(GrayscaleDecimation);
  SETCAMERAPROP_INT(Intensity);
  SETCAMERAPROP_INT(PrecisionCap);
  SETCAMERAPROP_INT(ShutterDelay);
  SETCAMERAPROP_INT(StatusIntensity);
  SETCAMERAPROP_INT(Threshold);
  SETCAMERAPROP_STR(Name);
#undef SETCAMERAPROP_BOOL
#undef SETCAMERAPROP_INT
#undef SETCAMERAPROP_STR

  d=-1;
  if(props.get("quality", d)) {
    int quality=d;
    if(quality!=m_quality) {
      m_quality=quality;
      if(m_quality<0) {
        m_camera->SetVideoType(GrayscaleMode);
      } else {
        m_camera->SetVideoType(MJPEGMode);
        m_camera->SetMJPEGQuality(m_quality);
      }
    }
  }

  /*
    SetAllLED(eStatusLEDs);
    SetLED(eStatusLEDs, bool);

    SetVideoType(eVideoMode);
    SetLateMJPEGDecompression(bool);
    SetMJPEGQuality(int);

    SetName(const char*);
    SetNumeric(bool, int);

    SetWindow(int, int, int, int);
    SetObjectColor(int);
    SetBitMaskPixel(int x, int y, bool);

    SetCameraParameter(char*name, float value);
  */
}


void videoOptiTrack::getProperties(gem::Properties&props)
{
  std::vector<std::string>keys=props.keys();
  double d;
  std::string s;

  props.clear();
  if(!m_camera) {
    return;
  }

  for(unsigned int i=0; i<keys.size(); i++) {
    const std::string key=keys[i];
    if("width"==key) {
      props.set(key, m_pixBlock.image.xsize);
      continue;
    }
    if("height"==key) {
      props.set(key, m_pixBlock.image.ysize);
      continue;
    }
    if("fanspeed"==key && m_camera->IsCameraFanSpeedValid()) {
      d=m_camera->CameraFanSpeed();
      props.set(key, d);
      continue;
    }
    if("temperature"==key && m_camera->IsCameraTempValid()) {
      d=m_camera->CameraTemp();
      props.set(key, d);
      continue;
    }

#define GETCAMERAPROP_BOOL(name) if(#name == key) {d=m_camera->name(); props.set(key, d); continue; } else d=0
#define GETCAMERAPROP_INT(name)  if(#name == key) {d=m_camera->name(); props.set(key, d); continue; } else d=0
#define GETCAMERAPROP_STR(name)  if(#name == key) {s=m_camera->name(); props.set(key, s); continue; } else d=0
    GETCAMERAPROP_BOOL(AEC);
    GETCAMERAPROP_BOOL(AGC);
    GETCAMERAPROP_BOOL(ContinuousIR);
    GETCAMERAPROP_BOOL(HighPowerMode);
    GETCAMERAPROP_BOOL(IRFilter);
    GETCAMERAPROP_BOOL(MarkerOverlay);
    GETCAMERAPROP_BOOL(TextOverlay);
    GETCAMERAPROP_INT(Exposure);
    GETCAMERAPROP_INT(FrameDecimation);
    GETCAMERAPROP_INT(FrameRate);
    GETCAMERAPROP_INT(GrayscaleDecimation);
    GETCAMERAPROP_INT(Intensity);
    GETCAMERAPROP_INT(PrecisionCap);
    GETCAMERAPROP_INT(ShutterDelay);
    GETCAMERAPROP_INT(Threshold);
    GETCAMERAPROP_STR(Name);
#undef GETCAMERAPROP_BOOL
#undef GETCAMERAPROP_INT
#undef GETCAMERAPROP_STR
  }
}

std::vector<std::string>videoOptiTrack::dialogs(void)
{
  std::vector<std::string>result;
  return result;
}
bool videoOptiTrack::provides(const std::string&name)
{
  return (name==s_name);
}
std::vector<std::string>videoOptiTrack::provides(void)
{
  std::vector<std::string>result;
  result.push_back(s_name);
  return result;
}
const std::string videoOptiTrack::getName(void)
{
  return s_name;
}


bool videoOptiTrack::start(void)
{
  //MARK;
  bool status = false;
  if(m_camera) {
    m_camera->Start();
    status=m_camera->IsCameraRunning();
  }

  return status;
}
bool videoOptiTrack::stop (void)
{
  //MARK;
  bool status=false;
  if(m_camera) {
    status=m_camera->IsCameraRunning();
    m_camera->Stop();
  }
  return status;
}

bool videoOptiTrack::reset(void)
{
  CameraLibrary::CameraManager::X().Shutdown();
  CameraManager::X().WaitForInitialization();
  return true;
}

#endif /* HAVE_LIBOPTITRACK */
