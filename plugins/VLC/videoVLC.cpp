////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
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

#if 0
# define LOCK(x)   post("%04d\t%s LOCK", __LINE__, __FUNCTION__), x.unlock(), post("%04d\t%s LOCKed", __LINE__, __FUNCTION__)
# define UNLOCK(x) post("%04d\t%s UNLOCK", __LINE__, __FUNCTION__), x.unlock(), post("%04d\t%s UNLOCKed", __LINE__, __FUNCTION__)
#else
# define LOCK(x)   x.lock()
# define UNLOCK(x) x.unlock()
#endif

REGISTER_VIDEOFACTORY("vlc", videoVLC);

namespace
{
static const char*  format_string="RV32";
static const unsigned int format_enum  = GEM_RGBA;
};

videoVLC::videoVLC(void) :
  m_name(std::string("vlc")),
  m_convertImg(0),
  m_type(0),
  m_instance(NULL),
  m_mediaplayer(NULL)
{
  m_instance=libvlc_new (0, 0);
  if(!m_instance) {
    throw(GemException("couldn't initialize libVLC"));
  }
  resize(64,64,0);
}

videoVLC::~videoVLC(void)
{
  if(m_instance) {
    libvlc_release(m_instance);
  }

  if(&m_pixBlock.image == m_convertImg) {
    m_convertImg=0;
  } else if (m_convertImg) {
    delete m_convertImg;
    m_convertImg=0;
  }
}
void videoVLC::close(void)
{
  if(m_mediaplayer) {
    libvlc_media_player_release(m_mediaplayer);
  }
  m_mediaplayer=NULL;
}

bool videoVLC::open(gem::Properties&props)
{
  if(m_mediaplayer) {
    close();
  }
  m_pixBlock.image.xsize=0;
  m_pixBlock.image.ysize=0;

  setProperties(props);


  if(m_devname.empty()) {
    return false;
  }

  libvlc_media_t*media = libvlc_media_new_location (m_instance,
                         m_devname.c_str());
  if(!media) {
    media = libvlc_media_new_path (m_instance, m_devname.c_str());
  }

  if(!media) {
    return false;
  }

  char buf[MAXVLCSTRING];

  libvlc_media_add_option(media,":noaudio");
  libvlc_media_add_option(media,":no-video-title-show");


  int w=m_pixBlock.image.xsize;
  int h=m_pixBlock.image.ysize;


  std::vector<std::string>keys=props.keys();
  for(unsigned int i=0; i<keys.size(); i++) {
    std::string key=keys[i];
    double d;
    std::string s;
    buf[0]=0;
    if(0) {}
    else if("width"==key) {
      if(props.get(key, d)&&(d>0)) {
        w=d;
      }
    } else if("height"==key) {
      if(props.get(key, d)&&(d>0)) {
        h=d;
      }
    } else {
      gem::Properties::PropertyType type = props.type(key);
      switch(type) {
      case gem::Properties::NONE:
        snprintf(buf, MAXVLCSTRING, ":%s", key.c_str());
        break;
      case gem::Properties::DOUBLE:
        if(props.get(key, d)) {
          snprintf(buf, MAXVLCSTRING, ":%s=%g", key.c_str(), d);
        }
        break;
      case gem::Properties::STRING:
        if(props.get(key, s)) {
          /* need to find an option that actually takes strings, so i can test this with spaces */
          snprintf(buf, MAXVLCSTRING, ":%s=%s", key.c_str(), s.c_str());
        }
        break;
      default:
        break;
      }
      if(0!=buf[0]) {
        buf[MAXVLCSTRING-1]=0;
        libvlc_media_add_option(media,buf);
      }
    }

  }
  resize(w,h,0);
  m_pixBlock.image.setWhite();

  m_mediaplayer=libvlc_media_player_new_from_media(media);
  libvlc_media_release(media);

  /* helper classes to register callbacks */
  struct _callbackObj {
    static void*lock(void*opaque, void**plane )
    {
      videoVLC*obj=(videoVLC*)opaque;
      if(obj) {
        return obj->lockFrame(plane);
      }
      return NULL;
    }
    static void unlock(void*opaque, void*picture, void*const*plane)
    {
      videoVLC*obj=(videoVLC*)opaque;
      if(obj) {
        obj->unlockFrame(picture, plane);
      }
    }
    static void display(void*opaque, void*picture)
    {
      videoVLC*obj=(videoVLC*)opaque;
    }
    _callbackObj(videoVLC*data)
    {
      libvlc_video_set_callbacks(data->m_mediaplayer,
                                 lock,
                                 unlock,
                                 NULL,
                                 data);
    }
  };
  struct _formatCallbackObj {
    static unsigned format(void**opaque, char *chroma, unsigned *width,
                           unsigned *height, unsigned *pitches, unsigned *lines)
    {
      videoVLC**objptr=(videoVLC**)opaque;
      if(objptr && *objptr) {
        return (*objptr)->setFormat(chroma, *width, *height, *pitches, *lines);
      }
      return 0;
    }
    _formatCallbackObj(videoVLC*data)
    {
      libvlc_video_set_format_callbacks(data->m_mediaplayer,
                                        format,
                                        NULL
                                       );

    }
  };
  /* instantiate helper-classes (which registers callbacks) */
  _callbackObj(this);
  _formatCallbackObj(this);

  return true;
}

pixBlock*videoVLC::getFrame(void)
{
  LOCK(m_mutex);
  return &m_pixBlock;
}

void videoVLC::releaseFrame(void)
{
  UNLOCK(m_mutex);
}

std::vector<std::string>videoVLC::enumerate(void)
{
  std::vector<std::string>result;
  result.push_back("vlc");
  return result;
}

bool videoVLC::setDevice(int ID)
{
  m_devname.clear();
  return false;
}
bool videoVLC::setDevice(const std::string&device)
{
  m_devname=device;
  return true;
}
bool videoVLC::enumProperties(gem::Properties&readable,
                              gem::Properties&writeable)
{
  readable.clear();
  writeable.clear();

  writeable.set("width",  m_pixBlock.image.xsize);
  readable.set("width",  m_pixBlock.image.xsize);
  writeable.set("height",  m_pixBlock.image.ysize);
  readable.set("height",  m_pixBlock.image.ysize);
  return false;
}
void videoVLC::setProperties(gem::Properties&props)
{
  int width=-1;
  int height=-1;

  m_props=props;

  double d;
  if(props.get("width", d)) {
    if(d>0) {
      width = d;
    }
  }
  if(props.get("height", d)) {
    if(d>0) {
      height=d;
    }
  }

  if(!m_mediaplayer) {
    if(width>0) {
      m_pixBlock.image.xsize=width;
    }
    if(height>0) {
      m_pixBlock.image.ysize=height;
    }
  } else {
    // changes will take effect with next restart
  }
}


void videoVLC::getProperties(gem::Properties&props)
{
  std::vector<std::string>keys=props.keys();
  double d;

  for(int i=0; i<keys.size(); i++) {
    if("width"==keys[i]) {
      props.set(keys[i], m_pixBlock.image.xsize);
    }
    if("height"==keys[i]) {
      props.set(keys[i], m_pixBlock.image.ysize);
    }
  }
}

std::vector<std::string>videoVLC::dialogs(void)
{
  std::vector<std::string>result;
  return result;
}
bool videoVLC::provides(const std::string&name)
{
  return (name==m_name);
}
std::vector<std::string>videoVLC::provides(void)
{
  std::vector<std::string>result;
  result.push_back(m_name);
  return result;
}
const std::string videoVLC::getName(void)
{
  return m_name;
}


bool videoVLC::start(void)
{
  int ret=-1;
  if(m_mediaplayer) {
    int ret=libvlc_media_player_play(m_mediaplayer);
  }
  return (0!=ret);
}
bool videoVLC::stop (void)
{
  if(!m_mediaplayer) {
    return false;
  }
  libvlc_media_player_stop(m_mediaplayer);
  return true;
}

void*videoVLC::lockFrame(void**plane )
{
  LOCK(m_mutex);
  *plane=m_convertImg->data;

  return NULL;
}
void videoVLC::unlockFrame(void*picture, void*const*plane)
{
  if(&m_pixBlock.image != m_convertImg) {
    // convert the image from the buffer
#ifdef __APPLE__
    m_pixBlock.image.fromARGB(m_convertImg->data);
#else
    m_pixBlock.image.fromBGRA(m_convertImg->data);
#endif /* __APPLE__ */
  }

  m_pixBlock.newimage=true;
  m_pixBlock.image.upsidedown=true;
  UNLOCK(m_mutex);
}

unsigned videoVLC::setFormat(char chroma[4], unsigned &width,
                             unsigned &height, unsigned &pitches, unsigned &lines)
{
#if 0
  logpost(0, 3+1, "[GEM:videoVLC] chroma: %s", chroma);
  logpost(0, 3+1, "[GEM:videoVLC] dimen : %dx%d", width, height);
  logpost(0, 3+1, "[GEM:videoVLC] pitches: %d", pitches);
  logpost(0, 3+1, "[GEM:videoVLC] lines: %d", lines);
#endif
  memcpy(chroma, format_string, 4);

  if(m_pixBlock.image.xsize == 0 || m_pixBlock.image.ysize == 0 ) {
    resize(width, height, 0);
  } else {
    width  =m_pixBlock.image.xsize;
    height =m_pixBlock.image.ysize;
  }
  pitches=width*m_pixBlock.image.csize;
  lines=height;

  return 1;
}
void videoVLC::resize(unsigned int width, unsigned int height,
                      unsigned int format)
{
  bool do_convert = true;

  if(0==format) {
    format=format_enum;
  }

  m_pixBlock.image.xsize = width;
  m_pixBlock.image.ysize = height;
  m_pixBlock.image.setFormat(format);
  m_pixBlock.image.reallocate();

  if(&m_pixBlock.image == m_convertImg) {
    m_convertImg=0;
  } else if (m_convertImg) {
    delete m_convertImg;
    m_convertImg=0;
  }

  if(do_convert) {
    m_convertImg=new imageStruct;
    m_pixBlock.image.copy2ImageStruct(m_convertImg);
    m_convertImg->allocate();
  } else {
    m_convertImg=&m_pixBlock.image;
  }
}
#endif /* HAVE_LIBVLC */
