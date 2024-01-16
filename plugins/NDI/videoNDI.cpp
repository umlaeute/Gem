////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2010-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "LICENSE.txt"
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "videoNDI.h"
#include "plugins/PluginFactory.h"

#include <sstream>
using namespace gem::plugins;

#include "Gem/RTE.h"
#include "Gem/Exception.h"

#if 0
# define debug ::post
#else
# include "Utils/nop.h"
# define debug nop_post
#endif

#include "init_ndi_library.hh"
namespace
{
const NDIlib_gem* NDI = 0;
};


/////////////////////////////////////////////////////////
//
// videoNDI
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

REGISTER_VIDEOFACTORY("NDI", videoNDI);

videoNDI :: videoNDI()
  : m_ndi_recv(NULL)
  , m_ndi_find(NULL)
  , m_name(std::string("ndi"))
  , m_devicename(std::string(""))
  , m_devicenum(0)
  , m_async(true)
  , m_gotone(false)
{
  MARK();
  if(!NDI) {
    NDI = init_ndi_library("videoNDI");
  }

  if(!NDI) {
    throw(GemException("couldn't initialize libNDI"));
  }

  if (!NDI->initialize()) {
    if(!NDI->is_supported_CPU())
      throw(GemException("NDI failed to initialize: unsupported CPU!"));
    throw(GemException("NDI failed to initialize!"));
  }

  m_ndi_find = NDI->find_create_v2(NULL);

  m_provides.push_back(m_name);
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
videoNDI :: ~videoNDI()
{
  MARK();
  close();
  NDI->destroy();
}

const std::string videoNDI::getName(void)
{
  MARK();
  return m_name;
}
bool videoNDI::provides(const std::string&name)
{
  MARK();
  for(unsigned int i=0; i<m_provides.size(); i++)
    if(name == m_provides[i]) {
      return true;
    }
  return false;
}
std::vector<std::string>videoNDI::provides(void)
{
  MARK();
  return m_provides;
}

bool videoNDI::enumProperties(gem::Properties&readable,
                              gem::Properties&writeable)
{
  int dummy_i;

  readable.clear();
  writeable.clear();
  dummy_i = 0;
  writeable.set("future", dummy_i);

  return true;
}
void videoNDI::setProperties(gem::Properties&props)
{ }

void videoNDI::getProperties(gem::Properties&props)
{
  std::vector<std::string>keys=props.keys();
  for(int i=0; i<keys.size(); i++) {
    std::string key = keys[i];
    if (0) {
    } else if("width" == key) {
      props.set(key, m_ndi_frame.xres);
    } else if("height" == key) {
      props.set(key, m_ndi_frame.yres);
    } else if("aspect_ratio" == key) {
      props.set(key, m_ndi_frame.picture_aspect_ratio);
    } else {
      props.erase(key);
    }
  }
}

std::vector<std::string> videoNDI::enumerate()
{
  std::vector<std::string> result;
  if (!m_ndi_find) { /* Failed to create finder */
    return result;
  }
  uint32_t no_srcs; // This will contain how many senders have been found so far.
  const NDIlib_source_t* p_senders = NDI->find_get_current_sources(m_ndi_find, &no_srcs);
  if((no_srcs<1) || !p_senders) {
    return result;
  }

  for(uint32_t i=0; i<no_srcs; i++) {
    result.push_back(p_senders[i].p_ndi_name);
    verbose(1, "[GEM:videoNDI] device#%d\t%s\t%s", i, p_senders[i].p_ndi_name, p_senders[i].p_url_address);
    //result.push_back(p_senders[i].p_url_address);
  }
  return result;
}

bool videoNDI::setDevice(int ID)
{
  MARK();
  return false;
}
bool videoNDI::setDevice(const std::string&device)
{
  MARK();
  m_devicename=device;
  return true;
}


/////////////////////////////////////////////////////////
// openDevice
//
/////////////////////////////////////////////////////////

bool videoNDI :: open(gem::Properties&props)
{
  MARK();
  if(m_devicename.empty()) {
    return false;
  }
  bool future = false;
  double dval;
  if(props.type("future") != gem::Properties::UNSET) {
    if(props.get("future", dval)) {
      future = (int)dval > 0;
    }
  }
  post("[GEM:videoNDI] future = %d", future);
  if(!future) {
    if (!m_ndi_find) {
      return false;
    }
    uint32_t no_srcs; // This will contain how many senders have been found so far.
    const NDIlib_source_t* p_senders = NDI->find_get_current_sources(m_ndi_find, &no_srcs);
    if((no_srcs<1) || !p_senders) {
      return false;
    }
    uint32_t src_num = 0;
    for(src_num=0; src_num<no_srcs; src_num++) {
      if (p_senders[src_num].p_ndi_name == m_devicename || p_senders[src_num].p_url_address == m_devicename) {
        break;
      }
    }
    if(src_num >= no_srcs) {
      verbose(1, "[GEM:videoNDI] device '%s' does not exist, skipping", m_devicename.c_str());
      return false;
    }
  }
  verbose(1, "[GEM:videoNDI] opening device '%s'", m_devicename.c_str());

  m_ndi_recv = NDI->recv_create_v3(NULL);

  NDIlib_source_t my_source;
  my_source.p_ndi_name = m_devicename.c_str();
  my_source.p_url_address = m_devicename.c_str();

  NDI->recv_connect(m_ndi_recv, &my_source);

  /* we unset the 'future' property, so it doesn't apply to the next open();
   * we only do this if we actually managed to open the device
   */
  props.erase("future");
  return true;
}
/////////////////////////////////////////////////////////
// closeDevice
//
/////////////////////////////////////////////////////////
void videoNDI :: close()
{
  MARK();
  if(m_ndi_recv) {
    NDI->recv_destroy(m_ndi_recv);
  }
  m_ndi_recv = NULL;
}


/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
bool videoNDI :: start()
{
  m_pixBlock.newfilm = 1;
  return true;
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
bool videoNDI :: stop()
{
  return true;
}

bool videoNDI :: reset()
{
  MARK();
  verbose(0, "[GEM:videoNDI] 'reset' not implemented");
  return false;
}

////////////////////////////////////////////////////////
// frame grabber
//
/////////////////////////////////////////////////////////
pixBlock*videoNDI::getFrame(void)
{
  switch(NDI->recv_capture_v2(m_ndi_recv, &m_ndi_frame, NULL, NULL, 0)) {
  case NDIlib_frame_type_video:
    m_pixBlock.image.xsize = m_ndi_frame.xres;
    m_pixBlock.image.ysize = m_ndi_frame.yres;
    switch (m_ndi_frame.FourCC) {
    case NDIlib_FourCC_video_type_UYVY:
    case NDIlib_FourCC_video_type_UYVA:
      m_pixBlock.image.format = GEM_YUV;
      break;
    case NDIlib_FourCC_video_type_RGBA:
    case NDIlib_FourCC_video_type_RGBX:
    case NDIlib_FourCC_video_type_BGRA:
    case NDIlib_FourCC_video_type_BGRX:
      m_pixBlock.image.format = GEM_RGBA;
      break;
    case NDIlib_FourCC_video_type_P216:
    case NDIlib_FourCC_video_type_PA16:
      verbose(1, "[GEM:videoNDI] unknown format P...");
      return NULL;
    case NDIlib_FourCC_video_type_YV12:
    case NDIlib_FourCC_video_type_I420:
    case NDIlib_FourCC_video_type_NV12:
      verbose(1, "[GEM:videoNDI] unknown format Y...");
      return NULL;
    default:
      verbose(1, "[GEM:videoNDI] unknown format...");
      return NULL;
    }
    m_pixBlock.image.setFormat();
    m_pixBlock.image.data = m_ndi_frame.p_data;
    m_pixBlock.image.not_owned = 1;
    m_pixBlock.newimage = 1;

    m_gotone = true;

    break;
  case NDIlib_frame_type_error:
    pd_error(0, "[GEM:videoNDI] lost connection");
    return NULL;
  case NDIlib_frame_type_audio:
    verbose(2, "[GEM:videoNDI] got audio frame");
    break;
  case NDIlib_frame_type_none:
  default:
    //pd_error(0, "[GEM:videoNDI] got no frame");
    break;
  } // switch

  return &m_pixBlock;
}
void videoNDI::releaseFrame(void)
{
  if(m_ndi_recv && m_gotone) {
    NDI->recv_free_video_v2(m_ndi_recv, &m_ndi_frame);
  }
  m_pixBlock.newimage = 0;
  m_pixBlock.newfilm = 0;
  m_gotone = false;
}


bool videoNDI::grabAsynchronous(bool async)
{
  MARK();
  bool ret = m_async;
  m_async = async;
  return ret;
}
bool videoNDI::setColor(int format)
{
  MARK();
  return false;
}
