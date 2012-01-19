////////////////////////////////////////////////////////
//
// videoOptiTrack - Graphics Environment for Multimedia
//
// daniel@bogusfront.org
// zmoelnig@iem.at
//
// Implementation file 
//
//    Copyright (c) 2003 Daniel Heckenberg.
//    Copyright (c) 2010-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#ifdef HAVE_LIBOPTITRACK

#include "videoOptiTrack.h"
using namespace gem::plugins;
#include "Gem/RTE.h"
#include "Gem/Exception.h"

#include "plugins/PluginFactory.h"

REGISTER_VIDEOFACTORY("OptiTrack", videoOptiTrack);


using namespace CameraLibrary;

const std::string videoOptiTrack::s_name = std::string("OptiTrack");

videoOptiTrack::videoOptiTrack(void)
{

  //  CameraManager::X().WaitForInitialization();
  throw(GemException("couldn't initialize OptiTrack"));

  m_pixBlock.image.xsize = 64;
  m_pixBlock.image.ysize = 64;
  m_pixBlock.image.setCsizeByFormat(GL_RGBA);
  m_pixBlock.image.reallocate();
}

videoOptiTrack::~videoOptiTrack(void) {
}
void videoOptiTrack::close(void) {
}

bool videoOptiTrack::open(gem::Properties&props) {
  return false;
}

pixBlock*videoOptiTrack::getFrame(void) {
  return &m_pixBlock;
}

void videoOptiTrack::releaseFrame(void) {
}

std::vector<std::string>videoOptiTrack::enumerate(void) {
  std::vector<std::string>result;
  //  result.push_back("vlc");
  return result;
}

bool videoOptiTrack::setDevice(int ID) {
  m_devname.clear();
  return false;
}
bool videoOptiTrack::setDevice(std::string device) {
  m_devname=device;
  return true;
}
bool videoOptiTrack::enumProperties(gem::Properties&readable,
			       gem::Properties&writeable) {
  readable.clear();
  writeable.clear();

  writeable.set("width",  m_pixBlock.image.xsize);  readable.set("width",  m_pixBlock.image.xsize);
  writeable.set("height",  m_pixBlock.image.ysize); readable.set("height",  m_pixBlock.image.ysize);
  return false;
}
void videoOptiTrack::setProperties(gem::Properties&props) {
  int width=-1;
  int height=-1;

  m_props=props;

  double d;
  if(props.get("width", d)) {
    if(d>0)
      width = d;
  }
  if(props.get("height", d)) {
    if(d>0)
      height=d;
  }

#if 0
  if(!m_mediaplayer) {
    if(width>0)
      m_pixBlock.image.xsize=width;
    if(height>0)
      m_pixBlock.image.ysize=height;
  } else {
    // changes will take effect with next restart
  }
#endif
}


void videoOptiTrack::getProperties(gem::Properties&props) {
  std::vector<std::string>keys=props.keys();
  double d;
  int i;

  for(i=0; i<keys.size(); i++) {
    if("width"==keys[i]) {
      props.set(keys[i], m_pixBlock.image.xsize);
    }
    if("height"==keys[i]) {
      props.set(keys[i], m_pixBlock.image.ysize);
    }
  }
}

std::vector<std::string>videoOptiTrack::dialogs(void) {
  std::vector<std::string>result;
  return result;
}
bool videoOptiTrack::provides(const std::string name) {
  return (name==s_name);
}
std::vector<std::string>videoOptiTrack::provides(void) {
  std::vector<std::string>result;
  result.push_back(s_name);
  return result;
}
const std::string videoOptiTrack::getName(void) {
  return s_name;
}


bool videoOptiTrack::start(void) {
  return false;
}
bool videoOptiTrack::stop (void) {
  return false;
}
#endif /* HAVE_LIBOPTITRACK */
