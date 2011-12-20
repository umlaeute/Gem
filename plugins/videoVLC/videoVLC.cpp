
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

#include <stdio.h>
#ifdef _MSC_VER
# define snprintf _snprintf
#endif

#include "videoVLC.h"
#include "plugins/PluginFactory.h"

#include "Gem/Exception.h"
#include "Gem/RTE.h"

#define MAXVLCSTRING 1024

using namespace gem::plugins;

REGISTER_VIDEOFACTORY("vlc", videoVLC);

videoVLC::videoVLC() :
  m_name(std::string("vlc")),
  m_type(0),
  m_instance(NULL),
  m_mediaplayer(NULL)
{
  const char * const vlc_args[] = {
    //    "--plugin-path=c:\\program files\\videolan\\vlc\\plugins",
    "-I", "dummy", /* Don't use any interface */
    "--ignore-config", /* Don't use VLC's config */
    "--quiet",
    //    "--sout=#transcode{vcodec=RV24,acodec=s16l}:smem",
  };
  m_instance=libvlc_new (sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);
  if(!m_instance) {
    throw(GemException("couldn't initialize libVLC"));
  }

  m_pixBlock.image.xsize = 64;
  m_pixBlock.image.ysize = 64;
  m_pixBlock.image.setCsizeByFormat(GL_RGBA);
  m_pixBlock.image.reallocate();
}

videoVLC::~videoVLC(void) {
  if(m_instance)
    libvlc_release(m_instance);

}
void videoVLC::close(void) {
  if(m_mediaplayer)
    libvlc_media_player_release(m_mediaplayer);
  m_mediaplayer=NULL;
}

bool videoVLC::open(gem::Properties&props) {
  setProperties(props);

  if(m_mediaplayer)close();

  if(m_devname.empty())
    return false;

  libvlc_media_t*media = libvlc_media_new_location (m_instance, m_devname.c_str());
  if(!media)
    media = libvlc_media_new_path (m_instance, m_devname.c_str());

  if(!media)
    return false;

  char s[MAXVLCSTRING];

  libvlc_media_add_option(media,":noaudio");
  libvlc_media_add_option(media,":no-video-title-show");

  m_mediaplayer=libvlc_media_player_new_from_media(media);
  libvlc_media_release(media);

  libvlc_video_set_callbacks(m_mediaplayer,
                             lockCB,
                             unlockCB,
                             NULL,
                             this);

  m_pixBlock.image.setCsizeByFormat(GL_RGBA);
  m_pixBlock.image.reallocate();
  m_pixBlock.image.setWhite();

  libvlc_video_set_format(m_mediaplayer,
                          "RV32",
                          m_pixBlock.image.xsize,
                          m_pixBlock.image.ysize,
                          m_pixBlock.image.xsize*m_pixBlock.image.csize);
  return true;
}

pixBlock*videoVLC::getFrame(void) {
  m_mutex.lock();
  return &m_pixBlock;
}

void videoVLC::releaseFrame(void) {
  //  post("release frame");
  m_mutex.unlock();
}

std::vector<std::string>videoVLC::enumerate(void) {
  std::vector<std::string>result;
  result.push_back("vlc");
  return result;
}

bool videoVLC::setDevice(int ID) {
  m_devname.clear();
  return false;
}
bool videoVLC::setDevice(std::string device) {
  m_devname=device;
  return true;
}
bool videoVLC::enumProperties(gem::Properties&readable,
			       gem::Properties&writeable) {
  readable.clear();
  writeable.clear();

  writeable.set("width", 64);  readable.set("width", 64);
  writeable.set("height", 64); readable.set("height", 64);
  return false;
}
void videoVLC::setProperties(gem::Properties&props) {
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
}
void videoVLC::getProperties(gem::Properties&props) {
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


bool videoVLC::start(void) {
  int ret=-1;
  if(m_mediaplayer) {
    int ret=libvlc_media_player_play(m_mediaplayer);
  }
  return (0!=ret);
}
bool videoVLC::stop (void) {
  if(!m_mediaplayer)
    return false;
  libvlc_media_player_stop(m_mediaplayer);
  return true;
}

void*videoVLC::lockFrame(void**plane ) {
  m_mutex.lock();
  *plane=m_pixBlock.image.data;
  //  post("prepareFrame %p @ %p --> %p", *plane, plane, m_pixBlock.image.data);

  return NULL;
}
void videoVLC::unlockFrame(void*picture, void*const*plane) {
  //  post("processFrame %p\t%p", picture, *plane);
  m_pixBlock.newimage=true;
  m_mutex.unlock();
}
void*videoVLC::lockCB(void*opaque, void**plane ) {
  //  post("   lockCB: %p", opaque);
  videoVLC*obj=(videoVLC*)opaque;
  if(obj)
    return obj->lockFrame(plane);
  
  return NULL;
}
void videoVLC::unlockCB(void*opaque, void*picture, void*const*plane) {
  //  post(" unlockCB: %p", opaque);
  videoVLC*obj=(videoVLC*)opaque;
  if(obj)
    obj->unlockFrame(picture, plane);
}
void videoVLC::displayCB(void*opaque, void*picture) {
  //  post("displayCB: %p -> %p", opaque, picture);
  videoVLC*obj=(videoVLC*)opaque;
}

#endif /* HAVE_LIBVLC */
