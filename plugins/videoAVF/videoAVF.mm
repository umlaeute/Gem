/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Implementation file

Copyright (c) 2016-2018 Dan Wilcox.
        2018 IOhannes m zmölnig

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

#define VIDEOAVF_DEFAULT_PIXELFORMAT GL_YCBCR_422_APPLE
#define VIDEOAVF_DEFAULT_WIDTH 1280
#define VIDEOAVF_DEFAULT_HEIGHT 720

void clear(void)
{
  error("clear(): unimplemented)")
}

videoAVF::videoAVF(void)
  : m_name(std::string("AVF")),
  , m_device(0),
  , m_videoGrabber(0)
  , m_wantedFormat(VIDEOAVF_DEFAULT_PIXELFORMAT)
  , m_readNext(false),
  , newFrame(false)
  , bHavePixelsChanged(false)
  , width(VIDEOAVF_DEFAULT_WIDTH)
  , height(VIDEOAVF_DEFAULT_HEIGHT)
  , device(0)
  , bIsInit(false),
  , fps(-1)
  , bLock(false)
{
  grabber = [AVFVideoGrabber alloc];
  m_pixBlock.image.xsize = width;
  m_pixBlock.image.ysize = height;
  m_pixBlock.image.setCsizeByFormat(m_wantedFormat);
  m_pixBlock.image.reallocate();
}

videoAVF::~videoAVF(void)
{
  close();
}


std::vector<std::string>videoAVF::enumerate(void)
{
  std::vector<std::string>result;
  result.push_back(m_name);
  return result;
}

bool videoAVF::enumProperties(gem::Properties&readable,
                              gem::Properties&writeable)
{
  readable.clear();
  writeable.clear();


  writeable.set("width", VIDEOAVF_DEFAULT_WIDTH);
  writeable.set("height", VIDEOAVF_DEFAULT_HEIGHT);
  writeable.set("fps", -1);


  return true;
}
void videoAVF::setProperties(gem::Properties&props)
{
  m_props=props;

  double d;
  if(props.get("width", d)) {
    if(d>0) {
      m_pixBlock.image.xsize = d;
    }
  }
  if(props.get("height", d)) {
    if(d>0) {
      m_pixBlock.image.ysize = d;
    }
  }
}
void videoAVF::getProperties(gem::Properties&props)
{
  std::vector<std::string>keys=props.keys();
  unsigned int i;
  for(i=0; i<keys.size(); i++) {
    if("width"==keys[i]) {
      props.set(keys[i], m_pixBlock.image.xsize);
    }
    if("height"==keys[i]) {
      props.set(keys[i], m_pixBlock.image.ysize);
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
  m_devices = [grabber listDevices];
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
bool videoAVF::open(const gem::Properties &props)
{
  int fps=-1, w=VIDEOAVF_DEFAULT_WIDTH, h=VIDEOAVF_DEFAULT_HEIGHT;
  double d;
#if 0
  if(props.get("colorspace", d)) {
    m_wantedFormat = (GLenum)d;
  }
#endif
  if(props.get("width", d) && d>=1.) {
    w = d;
  }
  if(props.get("height", d) && d>=1.) {
    h = d;
  }
  if(props.get("fps", d) && d>=1.) {
    fps = d;
  }

  // close and reset
  close();

  if( !m_videoGrabber) {
    m_videoGrabber = [AVFVideoGrabber alloc];
  }

  m_videoGrabber->grabberPtr = this;

  m_videoGrabber->setDevice(m_device);
  if(! [m_videoGrabber initCapture:fps capWidth:w capHeight:h] ) {
    return false;
  }


  //update the pixel dimensions based on what the camera supports
  width = m_videoGrabber->width;
  height = m_videoGrabber->height;

  clear();

  pixels.allocate(width, height, pixelFormat);
  pixelsTmp.allocate(width, height, pixelFormat);

  return true;
}

/////////////////////////////////////////////////////////
// close
//
/////////////////////////////////////////////////////////
void videoAVF::close(void)
{
  bLock = true;
  if(m_videoGrabber) {
    [m_videoGrabber stopCapture];
    [m_videoGrabber eraseGrabberPtr];
    [m_videoGrabber release];
    m_videoGrabber = nil;
    m_image.image.clear();
  }
  clear();
  bIsInit = false;
  width = 0;
  height = 0;
  fps		= -1;
  pixelFormat = OF_PIXELS_RGB;
  newFrame = false;
  bHavePixelsChanged = false;
  bLock = false;
}


bool videoAVF::start(void)
{
  if(!m_videoGrabber) {
    return false;
  }

  [m_videoGrabber startCapture];

  newFrame=false;
  bIsInit = true;
}
bool videoAVF::stop(void)
{
  if(!bIsInit) {
    return false;
  }
  if(!m_videoGrabber) {
    return false;
  }

  [m_videoGrabber stopCapture];
  bIsInit = false;
  return true;
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
  post("getFrame: TODO");
#if 0
  // nothing to process?
  if(m_readNext == false) {
    return &m_image;
  }

  // grab frame into GEM image buffer
  CVImageBufferRef imageBuffer = [m_videoGrabber getFrame];
  if(!m_videoGrabber.isFrameNew) {
    m_readNext = false;
    return &m_image;
  }

  // lock buffer
  CVPixelBufferLockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);

  // copy pixels
  uint8_t *src = (uint8_t *)CVPixelBufferGetBaseAddress(imageBuffer);
  uint8_t *dest = m_image.image.data;
  size_t size = CVPixelBufferGetHeight(imageBuffer) *
                CVPixelBufferGetBytesPerRow(imageBuffer);
  if(src) {
    switch(m_wantedFormat) {
    case GL_YCBCR_422_APPLE:
    case GL_RGBA_GEM:
      // format should be correct, so just pass through
      memcpy(dest, src, size);
      break;
    case GL_LUMINANCE: {
      // copy every second byte (Y-channel) for grayscale
      src =  (uint8_t *)CVPixelBufferGetBaseAddressOfPlane(imageBuffer, 0);
      uint8_t *srcPos = (uint8_t *)src + 1;
      uint8_t *destPos = (uint8_t *)dest;
      uint8_t *destEnd = (uint8_t *)dest + size/2;
      while(destPos <= destEnd) {
        memcpy(destPos, srcPos, 1);
        srcPos += 2;
        destPos += 1;
      }
      break;
    }
    default:
      error("videoAVF: Unable to convert frame pixels, "
            "unknown format %d", (int)m_wantedFormat);
      break;
    }
  } else {
    error("videoAVF: Unable to convert frame pixels, "
          "source buffer is null");
  }

  // done, unlock buffer
  CVPixelBufferUnlockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);

  // done
  m_image.newimage = true;
  m_readNext = false;
  return &m_image;
#else
  return 0;
#endif
}


/* ================================================= */
/* misc, TODO,... */
/* ================================================= */


bool dialog(std::vector<std::string>names)
{
  return false;
}
std::vector<std::string> videoAVF::dialogs(void)
{
  std::vector<std::string> result;
  return result;
}

bool videoAVF::grabAsynchronous(bool state)
{
  return false;
}
bool videoAVF::setColor(int col)
{
  return false;
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
