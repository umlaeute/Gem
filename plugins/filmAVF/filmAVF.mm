/*  -*- mode: ObjC; -*- */
/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Implementation file

Copyright (c) 2016-2018 Dan Wilcox.

For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "filmAVF.h"
#include "plugins/PluginFactory.h"
#include "Gem/Properties.h"
#include "Gem/RTE.h"
#include "Gem/Exception.h"

#import "AVFMoviePlayer.h"

using namespace gem::plugins;

REGISTER_FILMFACTORY("AVF", filmAVF);

#define FILMAVF_DEFAULT_PIXELFORMAT GEM_YUV

/////////////////////////////////////////////////////////
//
// filmAVF
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
filmAVF::filmAVF(void)
{
  close(); // default values
  m_wantedFormat = FILMAVF_DEFAULT_PIXELFORMAT;
  m_image.image.setFormat(m_wantedFormat);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
filmAVF::~filmAVF(void)
{
  close();
}

/////////////////////////////////////////////////////////
// open
//
/////////////////////////////////////////////////////////
bool filmAVF::open(const std::string &filename,
                   const gem::Properties &props)
{
  if(filename.empty()) {
    return false;
  }
  double d;
  if(props.get("colorspace", d)) {
    m_wantedFormat = (unsigned int)d;
  }

  // close and reset
  close();

  // create if not allocated yet
  if(!m_moviePlayer) {
    m_moviePlayer = [[AVFMoviePlayer alloc] init];
  }

  // load
  // TODO: figure out how to make async loading work with GEM?
  NSString *path = [NSString stringWithUTF8String:filename.c_str()];
  if(![m_moviePlayer openFile:path async:NO]) {
    return false;
  }

  // set up frame data
  m_image.image.xsize = m_moviePlayer.width;
  m_image.image.ysize = m_moviePlayer.height;
  m_image.newfilm = true;
  changeFormat(m_wantedFormat);

  // set up movie data
  m_readNext = true;
  if(m_moviePlayer.duration > 0) {
    m_fps = m_moviePlayer.frameRate;
    m_numFrames = m_moviePlayer.numFrames;
    m_numTracks = m_moviePlayer.numTracks;
  } else { // defaults
    m_fps = 30.f;
    m_numFrames = 0;
    m_numTracks = 0;
  }

  return true;
}

/////////////////////////////////////////////////////////
// close
//
/////////////////////////////////////////////////////////
void filmAVF::close(void)
{
  if(m_moviePlayer) {
    m_moviePlayer = nil;
    m_image.image.clear();
  }
  m_numFrames = 0;
  m_numTracks = 0;
  m_curFrame = -1;
  m_curTrack = -1;
  m_readNext = false;
  m_fps = -1.0;
}

/////////////////////////////////////////////////////////
// getFrame
//
/////////////////////////////////////////////////////////
pixBlock* filmAVF::getFrame(void)
{
  if(!m_moviePlayer || !m_moviePlayer.isLoaded) {
    return 0;
  }

  // nothing to process?
  if(m_readNext == false) {
    return &m_image;
  }

  // grab frame into GEM image buffer
  CVImageBufferRef imageBuffer = [m_moviePlayer getFrame];
  if(!m_moviePlayer.isFrameNew) {
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
    case GEM_YUV:
    case GEM_RGBA:
      // format should be correct, so just pass through
      memcpy(dest, src, size);
      break;
    case GEM_GRAY: {
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
      pd_error(0, "filmAVF: Unable to convert frame pixels, "
               "unknown format %d", (int)m_wantedFormat);
      break;
    }
  } else {
    pd_error(0, "filmAVF: Unable to convert frame pixels, "
             "source buffer is null");
  }

  // done, unlock buffer
  CVPixelBufferUnlockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);

  // done
  m_image.newimage = true;
  m_readNext = false;
  return &m_image;
}

/////////////////////////////////////////////////////////
// changeImage
//
/////////////////////////////////////////////////////////
film::errCode filmAVF::changeImage(int imgNum, int trackNum)
{
  m_readNext = false;
  if(imgNum == -1) {
    imgNum = m_curFrame;
  }
  if(m_numFrames > 1 && imgNum >= m_numFrames) {
    return film::FAILURE;
  }
  if(trackNum == -1 || trackNum > m_numTracks) {
    trackNum = m_curTrack;
  }
  if(!m_moviePlayer.isLoaded) {
    return film::SUCCESS;
  }
  [m_moviePlayer setFrame:imgNum andTrack:trackNum];
  m_curFrame = imgNum;
  m_readNext = true;
  return film::SUCCESS;
}

/////////////////////////////////////////////////////////
// Properties
//
/////////////////////////////////////////////////////////
bool filmAVF::enumProperties(gem::Properties &readable,
                             gem::Properties &writeable)
{
  readable.clear();
  writeable.clear();

  gem::any value;
  value = 0.0;
  readable.set("fps", value);
  readable.set("frames", value);
  readable.set("width", value);
  readable.set("height", value);

  writeable.set("colorspace", value);

  return false;
}

void filmAVF::setProperties(gem::Properties &props)
{
  double d;
  if(props.get("colorspace", d)) {
    changeFormat((unsigned int)d);
  }
}

void filmAVF::getProperties(gem::Properties &props)
{
  std::vector<std::string> keys = props.keys();
  gem::any value;
  double d;
  unsigned int i = 0;
  for(i = 0; i < keys.size(); i++) {
    std::string key = keys[i];
    props.erase(key);
    if("fps" == key) {
      d = m_fps;
      value = d;
      props.set(key, value);
    }
    if("frames" == key) {
      d = m_numFrames;
      value = d;
      props.set(key, value);
    }
    if("width" == key) {
      d = m_image.image.xsize;
      value = d;
      props.set(key, value);
    }
    if("height" == key) {
      d = m_image.image.ysize;
      value = d;
      props.set(key, value);
    }
  }
}

// PROTECTED

bool filmAVF::changeFormat(unsigned int format)
{
  bool changed = (m_wantedFormat != format);
  m_wantedFormat = format;
  if(m_moviePlayer) {
    switch(m_wantedFormat) {
    default:
    case GEM_YUV:
    case GEM_GRAY:
      m_moviePlayer.desiredPixelFormat = kCVPixelFormatType_422YpCbCr8;
      break;
    case GEM_RGBA:
      m_moviePlayer.desiredPixelFormat = kCVPixelFormatType_32ARGB;
      break;
    }
  }
  m_image.image.format = m_wantedFormat;
  m_image.image.setFormat();
  if(m_image.image.data) {
    m_image.image.reallocate();
  } else {
    m_image.image.allocate();
  }
  return changed;
}
