/*  -*- mode: ObjC; -*- */
/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Implementation file

Copyright (c) 2018 IOhannes m zmölnig

Adapted from the openFrameworks ofAVFoundationGrabber class:
Jordan C. Parsons, Paul Gafton, Ryuichi Yamamoto, Seb Lee-Delisle,
Arturo Castro & Theodore Watson 2016-2018

For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "videoAVF.h"
#include "plugins/PluginFactory.h"
#include "Gem/Properties.h"
#include "Gem/RTE.h"
#include "Gem/Exception.h"

#import "AVFVideoGrabber.h"

using namespace gem::plugins;

REGISTER_VIDEOFACTORY("AVF", videoAVF);

//#define VIDEOAVF_DEFAULT_PIXELFORMAT GL_YCBCR_422_APPLE
#define VIDEOAVF_DEFAULT_PIXELFORMAT GL_RGBA_GEM
#define VIDEOAVF_DEFAULT_WIDTH 1280
#define VIDEOAVF_DEFAULT_HEIGHT 720

videoAVF::videoAVF(void)
  : m_name(std::string("AVF"))
  , m_device(0)
  , m_videoGrabber(0)
  , m_wantedFormat(VIDEOAVF_DEFAULT_PIXELFORMAT)
  , m_width(VIDEOAVF_DEFAULT_WIDTH)
  , m_height(VIDEOAVF_DEFAULT_HEIGHT)
  , m_isInit(false)
{
  m_videoGrabber = [AVFVideoGrabber alloc];
}

videoAVF::~videoAVF(void)
{
  close();
}


bool videoAVF::enumProperties(gem::Properties&readable,
                              gem::Properties&writeable)
{
  readable.clear();
  writeable.clear();

  writeable.set("width", m_width);
  writeable.set("height", m_height);
  writeable.set("fps", -1);


  return true;
}
void videoAVF::setProperties(gem::Properties&props)
{
  m_props=props;
}
void videoAVF::getProperties(gem::Properties&props)
{
  std::vector<std::string>keys=props.keys();
  unsigned int i;
  for(i=0; i<keys.size(); i++) {
    if(m_videoGrabber) {
      if("width"==keys[i]) {
        props.set(keys[i], m_width);
      }
      if("height"==keys[i]) {
        props.set(keys[i], m_height);
      }
    }
  }
}

///////////
// enumerate devices
std::vector<std::string> videoAVF::enumerate(void)
{
  if(!m_videoGrabber) {
    m_videoGrabber = [AVFVideoGrabber alloc];
  }
  m_devices = [m_videoGrabber listDevices];
  std::vector<std::string>result;
  for(int i=0; i<m_devices.size(); i++) {
    result.push_back(m_devices[i]);
  }
  return result;
}
bool videoAVF::setDevice(int ID)
{
  if(ID<0) {
    return false;
  }
  if (!m_devices.size()) {
    enumerate();
  }
  if(ID>=m_devices.size()) {
    return false;
  }
  m_device = ID;
  return true;
}
bool videoAVF::setDevice(const std::string&devname)
{
  if (!m_devices.size()) {
    enumerate();
  }
  for(int i=0; i<m_devices.size(); i++) {
    if(devname == m_devices[i]) {
      m_device = i;
      return true;
    }
  }
  return false;
}

/////////////////////////////////////////////////////////
// open/close
//
/////////////////////////////////////////////////////////
bool videoAVF::open(gem::Properties &props)
{
  m_props = props;
  if (!m_devices.size()) {
    enumerate();
  }
  return (m_device>=0 && m_device < m_devices.size());

}
bool videoAVF::start(void)
{
  int fps=-1, w=VIDEOAVF_DEFAULT_WIDTH, h=VIDEOAVF_DEFAULT_HEIGHT;
  double d;

  if(m_props.get("width", d) && d>=1.) {
    w = d;
  }
  if(m_props.get("height", d) && d>=1.) {
    h = d;
  }
  if(m_props.get("fps", d) && d>=1.) {
    fps = d;
  }

  // close and reset
  close();

  if( !m_videoGrabber) {
    m_videoGrabber = [AVFVideoGrabber alloc];
  }
  if( !m_videoGrabber) {
    return false;
  }

  [m_videoGrabber setDevice:m_device];
  if(! [m_videoGrabber initCapture:fps capWidth:w capHeight:h capFormat:
                       m_wantedFormat] ) {
    return false;
  }

  //update the pixel dimensions based on what the camera supports
  m_width = m_videoGrabber->width;
  m_height = m_videoGrabber->height;

  [m_videoGrabber startCapture];

  m_isInit = true;

  return true;
}

/////////////////////////////////////////////////////////
// close
//
/////////////////////////////////////////////////////////
bool videoAVF::stop(void)
{
  bool isinit = m_isInit;
  if(m_videoGrabber) {
    [m_videoGrabber stopCapture];
    //[m_videoGrabber release];
    m_videoGrabber = nil;
  }
  m_isInit = false;
  m_width = 0;
  m_height = 0;
  return isinit;
}

void videoAVF::close(void)
{
  stop();
}


/////////////////////////////////////////////////////////
// getFrame
//
/////////////////////////////////////////////////////////
pixBlock* videoAVF::getFrame(void)
{
  if(!m_videoGrabber) {
    return 0;
  }
  m_videoGrabber->lock.lock();
  pixBlock*img = &[m_videoGrabber getCurrentFrame];
  if(img) {
    img->newfilm = (m_width != img->image.xsize) || (m_height != img->image.ysize);
    m_width = img->image.xsize;
    m_height = img->image.ysize;
  }
  return img;
}
void videoAVF::releaseFrame(void)
{
  if(!m_videoGrabber) {
    return;
  }
  m_videoGrabber->lock.unlock();
}

bool videoAVF::grabAsynchronous(bool state)
{
  return true;
}

/* ================================================= */
/* misc, TODO,... */
/* ================================================= */


bool videoAVF::dialog(std::vector<std::string>names)
{
  return false;
}
std::vector<std::string> videoAVF::dialogs(void)
{
  std::vector<std::string> result;
  return result;
}

bool videoAVF::setColor(int col)
{
  m_wantedFormat = col;
  return(!m_isInit);
}

bool videoAVF::provides(const std::string&name)
{
  return (name==m_name);
}
std::vector<std::string>videoAVF::provides(void)
{
  std::vector<std::string>result;
  result.push_back(m_name);
  return result;
}
const std::string videoAVF::getName(void)
{
  return m_name;
}
bool videoAVF::reset(void)
{
  return false;
}
