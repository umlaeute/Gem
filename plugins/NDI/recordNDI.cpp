////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "recordNDI.h"

// for logpost(0, 3+):
#include "Gem/RTE.h"
#include "Gem/Exception.h"


#include "plugins/PluginFactory.h"


#include "init_ndi_library.hh"
namespace
{
const NDIlib_gem* NDI = 0;
};

using namespace gem::plugins;


REGISTER_RECORDFACTORY("NDI", recordNDI);
/////////////////////////////////////////////////////////
//
// recordNDI
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

recordNDI :: recordNDI(void)
  :  m_ndi_send(NULL)
{
  if(!NDI) {
    NDI = init_ndi_library("recordNDI");
  }

  if(!NDI) {
    throw(GemException("couldn't initialize libNDI"));
  }

  if (!NDI->initialize()) {
    if(!NDI->is_supported_CPU())
      throw(GemException("NDI failed to initialize: unsupported CPU!"));
    throw(GemException("NDI failed to initialize!"));
  }
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
recordNDI :: ~recordNDI(void)
{
  stop();
  NDI->destroy();
}

void recordNDI :: stop(void)
{
  if (m_ndi_send) {
    NDI->send_destroy(m_ndi_send);
  }
  m_ndi_send = NULL;
}

bool recordNDI :: start(const std::string&filename, gem::Properties&props)
{
  stop();

  NDIlib_send_create_t send_create;
  send_create.p_ndi_name = filename.c_str();
  send_create.p_groups = NULL;
  send_create.clock_video = false;
  send_create.clock_audio = false;
  m_ndi_send = NDI->send_create(&send_create);
  post("[GEM::recordNDI] opened '%s' as '%s'", filename.c_str(), send_create.p_ndi_name);
  if (!m_ndi_send) {
    return false;
  }
  return true;
}

bool recordNDI::init(const imageStruct* dummyImage, const int framedur)
{
  return true;
}



/////////////////////////////////////////////////////////
// do the actual encoding and writing to file
//
/////////////////////////////////////////////////////////
bool recordNDI :: write(imageStruct*img)
{
  NDIlib_video_frame_v2_t video_frame_data;
  video_frame_data.xres = img->xsize;
  video_frame_data.yres = img->ysize;
  video_frame_data.p_data = img->data;
  video_frame_data.frame_rate_N = 0;
  video_frame_data.frame_rate_D = 0;
  video_frame_data.picture_aspect_ratio = 0;
  video_frame_data.frame_format_type = NDIlib_frame_format_type_progressive;
  video_frame_data.timecode = 0;

  switch (img->format) {
  case GEM_RGBA:
    video_frame_data.FourCC = NDIlib_FourCC_video_type_RGBA;
    break;
  case GEM_YUV:
    video_frame_data.FourCC = NDIlib_FourCC_video_type_UYVY;
    break;
  default:
    return false;
  }
  //m_image.fixUpDown();
  NDI->send_send_video_v2(m_ndi_send, &video_frame_data);
  return true;
}


static const std::string s_codec_name=std::string("ndi");
static const std::string s_codec_desc=std::string("Network Device Interface");



/////////////////////////////////////////////////////////
// set codec by name
//
/////////////////////////////////////////////////////////
bool recordNDI :: setCodec(const std::string&name)
{
  if(name==s_codec_name) {
    return true;
  }
  return false;
}

/////////////////////////////////////////////////////////
// get codecs
//
/////////////////////////////////////////////////////////
std::vector<std::string>recordNDI::getCodecs(void)
{
  std::vector<std::string>result;
  result.push_back(s_codec_name);
  return result;
}
const std::string recordNDI::getCodecDescription(const std::string&codec)
{
  std::string result;
  if(codec==s_codec_name) {
    result=s_codec_desc;
  }
  return result;
}

bool recordNDI::enumProperties(gem::Properties&props)
{
  props.clear();
  return false;
}
