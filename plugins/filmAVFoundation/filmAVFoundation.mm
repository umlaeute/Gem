////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
//    Copyright (c) 2016 Dan Wilcox.
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "filmAVFoundation.h"
#include "plugins/PluginFactory.h"
#include "Gem/Properties.h"
#include "Gem/RTE.h"
#include "Gem/Exception.h"

#import "AVFMoviePlayer.h"

using namespace gem::plugins;

REGISTER_FILMFACTORY("AVFoundation", filmAVFoundation);

#define FILMAVFOUNDATION_DEFAULT_PIXELFORMAT GL_RGBA_GEM //GL_YUV422_GEM

/////////////////////////////////////////////////////////
//
// filmAVFoundation
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
filmAVFoundation::filmAVFoundation(void)
{
  close(); // default values
  m_wantedFormat = FILMAVFOUNDATION_DEFAULT_PIXELFORMAT;
  m_image.image.setCsizeByFormat(m_wantedFormat);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
filmAVFoundation::~filmAVFoundation(void)
{
  close();
}

/////////////////////////////////////////////////////////
// open
//
/////////////////////////////////////////////////////////
bool filmAVFoundation::open(const std::string &filename,
                            const gem::Properties &props)
{
  if(filename.empty()) {
    return false;
  }

  // close and reset
  close();

  // create if not allocated yet
  if(!m_moviePlayer) {
    m_moviePlayer = [[AVFMoviePlayer alloc] init];
  }

  // set desired format
  switch(m_wantedFormat) {
  default:
  case GL_YUV422_GEM:
    m_moviePlayer.desiredPixelFormat = kCVPixelFormatType_422YpCbCr8;
    break;
  case GL_RGBA_GEM:
    m_moviePlayer.desiredPixelFormat = kCVPixelFormatType_32ARGB;
    break;
  }

  // load
  NSString *path = [NSString stringWithUTF8String:filename.c_str()];
  if(![m_moviePlayer openFile:path async:NO]) {
    return false;
  }

  // set up frame data
  m_image.image.xsize = m_moviePlayer.width;
  m_image.image.ysize = m_moviePlayer.height;
  m_image.image.format = m_wantedFormat;
  m_image.image.setCsizeByFormat();
  m_image.image.allocate();
  m_image.newfilm = true;

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
void filmAVFoundation::close(void)
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
  //m_auto = 0.f;
  m_fps = -1.0;
  //m_newfilm = false;
}

/////////////////////////////////////////////////////////
// getFrame
//
/////////////////////////////////////////////////////////
pixBlock* filmAVFoundation::getFrame(void)
{
  if(!m_moviePlayer.isLoaded) {
    return 0;
  }

  // nothing to process?
  if(m_readNext == false) {
    return &m_image;
  }

  // grab frame into GEM image buffer
  CVImageBufferRef imageBuffer = [m_moviePlayer getFrame];

  // lock buffer
  CVPixelBufferLockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);

  // get buffer format & set buffer info for copying
  unsigned long imageBufferPixelFormat = CVPixelBufferGetPixelFormatType(
      imageBuffer);
  vImage_Buffer src = {
    CVPixelBufferGetBaseAddress(imageBuffer),
    CVPixelBufferGetHeight(imageBuffer),
    CVPixelBufferGetWidth(imageBuffer),
    CVPixelBufferGetBytesPerRow(imageBuffer)
  };
  vImage_Buffer dest = {
    m_image.image.data,
    static_cast<vImagePixelCount>(m_image.image.ysize),
    static_cast<vImagePixelCount>(m_image.image.xsize),
    static_cast<size_t>(m_image.image.xsize * m_image.image.csize)
  };
  vImage_Error err = kvImageNoError;

  // use Accelerate framework to convert formats and copy buffer
  if(imageBufferPixelFormat == kCVPixelFormatType_422YpCbCr8) {
    uint8_t permuteMap[4] = {0, 3, 2, 1};
    err = vImagePermuteChannels_ARGB8888(&src, &dest, permuteMap, 0);
  } else if(imageBufferPixelFormat == kCVPixelFormatType_32ARGB) {
    uint8_t permuteMap[4] = {0, 1, 2, 3};
    err = vImagePermuteChannels_ARGB8888(&src, &dest, permuteMap, 0);
  } else {
    error("filmAVFoundation: Unable to convert frame pixels, "
          "format %d is not 422 YUV or ARGB", (int)imageBufferPixelFormat);
  }

  // done, unlock buffer
  CVPixelBufferUnlockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);

  // check for errors
  if(err != kvImageNoError) {
    error("filmAVFoundation: Unable to convert frame pixels, vImage_error %d",
          (int)err);
  }

  // done
  m_image.newimage = true;
  m_readNext = false;
  return &m_image;
}

/////////////////////////////////////////////////////////
// changeImage
//
/////////////////////////////////////////////////////////
film::errCode filmAVFoundation::changeImage(int imgNum, int trackNum)
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
bool filmAVFoundation::enumProperties(gem::Properties &readable,
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

void filmAVFoundation::setProperties(gem::Properties &props)
{
  double d;
  // if(props.get("auto", d)) {
  //   m_auto = d;
  // }
  if(props.get("colorspace", d)) {
    m_wantedFormat = (GLenum)d;
  }
}

void filmAVFoundation::getProperties(gem::Properties &props)
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
