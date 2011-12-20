
////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#ifdef HAVE_LIBVLC


#include "videoVLC.h"
#include "plugins/PluginFactory.h"

#include "Gem/RTE.h"


using namespace gem::plugins;

REGISTER_VIDEOFACTORY("vlc", videoVLC);

videoVLC::videoVLC() :
  m_name(std::string("vlc")),
  m_open(false),
  m_type(0)
{
  m_pixBlock.image.xsize = 64;
  m_pixBlock.image.ysize = 64;
  m_pixBlock.image.setCsizeByFormat(GL_RGBA);
  m_pixBlock.image.reallocate();
}

videoVLC::~videoVLC(void) {
}

bool videoVLC::open(gem::Properties&props) {
  setProperties(props);
  return (m_open);
}

pixBlock*videoVLC::getFrame(void) {
  m_pixBlock.image.setCsizeByFormat(GL_RGBA);
  m_pixBlock.image.reallocate();

  return &m_pixBlock;
}

std::vector<std::string>videoVLC::enumerate(void) {
  std::vector<std::string>result;
  result.push_back("vlc");
  return result;
}

bool videoVLC::setDevice(int ID) {

  return m_open;
}
bool videoVLC::setDevice(std::string device) {

  return m_open;
}
bool videoVLC::enumProperties(gem::Properties&readable,
			       gem::Properties&writeable) {
  readable.clear();
  writeable.clear();

  /*
  writeable.set("width", 64);  readable.set("width", 64);
  writeable.set("height", 64); readable.set("height", 64);

  writeable.set("type", std::string("noise"));
  */
  return false;
}
void videoVLC::setProperties(gem::Properties&props) {
#if 0
  m_props=props;

  double d;
  if(props.get("width", d)) {
    if(d>0)
      m_pixBlock.image.xsize = d;
  }
  if(props.get("height", d)) {
    if(d>0)
      m_pixBlock.image.ysize = d;
  }
  std::string s;
  if(props.get("type", s)) {
    if("noise"==s)
      m_type=0;
    else if("red"==s)
      m_type=1;
    else if("green"==s)
      m_type=2;
    else if("blue"==s)
      m_type=3;
  }
#endif
}
void videoVLC::getProperties(gem::Properties&props) {
#if 0
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
#endif
}

std::vector<std::string>videoVLC::dialogs(void) {
  std::vector<std::string>result;
  return result;
}
bool videoVLC::provides(const std::string name) {
  return (name==m_name);
}
std::vector<std::string>videoVLC::provides(void) {
  std::vector<std::string>result;
  result.push_back(m_name);
  return result;
}
const std::string videoVLC::getName(void) {
  return m_name;
}


void videoVLC::close(void) {}
bool videoVLC::start(void) {return false;}
bool videoVLC::stop (void) {return false;}


void videoVLC::prepareFrame( uint8_t*&pp_pixel_buffer , int size ) {
  pp_pixel_buffer=NULL;
}
void videoVLC::processFrame(uint8_t*data, int width, int heigth, int pitch, int size, mtime_t pts) {

}

void videoVLC::prepareCB( void* p_video_data, uint8_t** pp_pixel_buffer , int size ) {
  videoVLC*obj=(videoVLC*)p_video_data;
  if(obj)
    obj->prepareFrame(*pp_pixel_buffer, size);
}
void videoVLC::processCB(void*p_video_data, uint8_t*data, int width, int heigth, int pitch, int size, mtime_t pts) {
  videoVLC*obj=(videoVLC*)p_video_data;
  if(obj)
    obj->processFrame(data, width, heigth, pitch, size, pts);
}


#endif /* HAVE_LIBVLC */
