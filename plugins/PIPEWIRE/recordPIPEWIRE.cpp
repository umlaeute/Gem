////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2022 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "recordPIPEWIRE.h"
#include "Gem/RTE.h"
#include "Gem/Manager.h"

#include "plugins/PluginFactory.h"

#include <spa/param/video/format-utils.h>
#include <spa/debug/types.h>

#include "m_pd.h"


using namespace gem::plugins;

#include <stdio.h>

REGISTER_RECORDFACTORY("pipewire", recordPIPEWIRE);

namespace
{
static struct pw_thread_loop *s_loop = NULL;
static unsigned int s_loopcount = 0;

bool recordPIPEWIRE_init(void)
{
  if(s_loop) {
    s_loopcount++;
    return false;
  }
  pw_init(0, 0);
  s_loop = pw_thread_loop_new("pd-gem", NULL);
  if(!s_loop) {
    pw_deinit();
    return false;
  }
  pw_thread_loop_start(s_loop);
  //::post("lop %p started", s_loop);
  s_loopcount = 1;
  return true;
}
void recordPIPEWIRE_deinit(void)
{
  if(!s_loopcount) {
    return;
  }
  if(--s_loopcount) {
    return;
  }
  pw_thread_loop_stop(s_loop);
  pw_thread_loop_destroy(s_loop);
  s_loop=0;
  pw_deinit();
}

spa_video_format format_gem2spa(unsigned int fmt)
{
  switch(fmt) {
  case GEM_RGB:
    return SPA_VIDEO_FORMAT_RGB;
  case GEM_RGBA:
    return SPA_VIDEO_FORMAT_RGBA;
  case GEM_YUV:
    return SPA_VIDEO_FORMAT_UYVY;
  case GEM_GRAY:
    return SPA_VIDEO_FORMAT_GRAY8;
  default:
    break;
  }
  return SPA_VIDEO_FORMAT_UNKNOWN;
}
}

/////////////////////////////////////////////////////////
//
// recordPIPEWIRE
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
recordPIPEWIRE :: recordPIPEWIRE(void)
  : record()
  , m_stream(nullptr)
{
  //::post("%s:%d@%s", __FILE__, __LINE__, __FUNCTION__);
  m_image.xsize = 640;
  m_image.ysize = 480;
  m_image.setFormat(GEM_RGBA);
  recordPIPEWIRE_init();
  m_stream_events = {
    PW_VERSION_STREAM_EVENTS,
    .param_changed = param_changed_cb,
    .process = process_cb,
  };
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
recordPIPEWIRE :: ~recordPIPEWIRE(void)
{
  //::post("%s:%d@%s", __FILE__, __LINE__, __FUNCTION__);
  stop();
}

void recordPIPEWIRE :: stop(void)
{
  //::post("%s:%d@%s", __FILE__, __LINE__, __FUNCTION__);
  if(!m_stream) {
    return;
  }
  pw_stream_set_active(m_stream, false);
  pw_stream_disconnect(m_stream);
  pw_stream_destroy(m_stream);
}

/////////////////////////////////////////////////////////
// open a file !
//
/////////////////////////////////////////////////////////

bool recordPIPEWIRE :: start(const std::string&filename, gem::Properties&props)
{
  //::post("%s:%d@%s", __FILE__, __LINE__, __FUNCTION__);
  if(!s_loop) {
    return false;
  }
  stop();
  //::post("%s:%d@%s", __FILE__, __LINE__, __FUNCTION__);
  m_filename = filename;
  if (m_filename.empty())
    m_filename = "gem-output";
  if(!m_image.xsize) {
    m_image.xsize = 640;
  }
  if(!m_image.ysize) {
    m_image.ysize = 480;
  }
  if(!m_image.csize) {
    m_image.setFormat(GEM_RAW_RGBA);
  }
  m_image.reallocate();

  int flags = PW_STREAM_FLAG_NONE;
  flags |= PW_STREAM_FLAG_INACTIVE;
  flags |= PW_STREAM_FLAG_MAP_BUFFERS;


  pw_thread_loop_lock(s_loop);
  m_stream = pw_stream_new_simple(
               pw_thread_loop_get_loop(s_loop),
               m_filename.c_str(),
               pw_properties_new(
                 PW_KEY_MEDIA_CLASS, "Video/Source",
                 PW_KEY_MEDIA_TYPE, "Video",
                 PW_KEY_MEDIA_ROLE, "Camera",
                 PW_KEY_APP_NAME, "Pd",
                 PW_KEY_APP_ID, "at.iem.gem",
                 PW_KEY_NODE_NAME, "Gem",
                 NULL),
               &m_stream_events,
               this);
  if(!m_stream) {
    //::post("%s:%d@%s", __FILE__, __LINE__, __FUNCTION__);
    pw_thread_loop_unlock(s_loop);
    return false;
  }

  uint8_t buffer[1024];
  const struct spa_pod *params[1];
  struct spa_pod_builder b = SPA_POD_BUILDER_INIT(buffer, sizeof(buffer));
  struct spa_rectangle defsize = SPA_RECTANGLE(640, 480);
  struct spa_rectangle maxsize = SPA_RECTANGLE(4096, 4096);
  struct spa_rectangle minsize = SPA_RECTANGLE(1,1);
  struct spa_fraction defrate  = SPA_FRACTION(25, 1);
  struct spa_fraction maxrate  = SPA_FRACTION(1000, 1);
  struct spa_fraction minrate  = SPA_FRACTION(0, 1);

  params[0] = (const spa_pod*)spa_pod_builder_add_object(&b,
              SPA_TYPE_OBJECT_Format, SPA_PARAM_EnumFormat,
              SPA_FORMAT_mediaType, SPA_POD_Id(SPA_MEDIA_TYPE_video),
              SPA_FORMAT_mediaSubtype, SPA_POD_Id(SPA_MEDIA_SUBTYPE_raw),
              SPA_FORMAT_VIDEO_format, SPA_POD_CHOICE_ENUM_Id(
                4
                , SPA_VIDEO_FORMAT_RGBA
                , SPA_VIDEO_FORMAT_RGB
                , SPA_VIDEO_FORMAT_YUY2
                //, SPA_VIDEO_FORMAT_UYVY
                , SPA_VIDEO_FORMAT_GRAY8
              ),
              SPA_FORMAT_VIDEO_size, SPA_POD_CHOICE_RANGE_Rectangle(
                &defsize, &minsize, &maxsize),
              SPA_FORMAT_VIDEO_framerate, SPA_POD_CHOICE_RANGE_Fraction(
                &defrate, &minrate, &maxrate));

  pw_stream_connect(m_stream,
                    PW_DIRECTION_OUTPUT,
                    PW_ID_ANY,
                    (pw_stream_flags)flags,
                    params, 1);


  pw_stream_set_active(m_stream, true);
  pw_thread_loop_unlock(s_loop);

  //::post("%s:%d@%s", __FILE__, __LINE__, __FUNCTION__);
  return (true);
}


bool recordPIPEWIRE :: init(imageStruct*img)
{
  ::post("setting format to %dx%d/0x%X", img->xsize, img->ysize, img->format);
  do {
    const struct spa_pod *params[1];
    uint8_t buffer[1024];
    struct spa_pod_builder b = SPA_POD_BUILDER_INIT(buffer, sizeof(buffer));
    spa_video_format defformat = format_gem2spa(img->format);
    struct spa_rectangle size = SPA_RECTANGLE((uint32_t)img->xsize, (uint32_t)img->ysize);
    struct spa_fraction framerate  = SPA_FRACTION(25, 1);
    //::post("%s:%d@%s", __FILE__, __LINE__, __FUNCTION__);

    params[0] = (const spa_pod*)spa_pod_builder_add_object(&b,
                SPA_TYPE_OBJECT_Format, SPA_PARAM_EnumFormat,
                SPA_FORMAT_mediaType,       SPA_POD_Id(SPA_MEDIA_TYPE_video),
                SPA_FORMAT_mediaSubtype,    SPA_POD_Id(SPA_MEDIA_SUBTYPE_raw),
                SPA_FORMAT_VIDEO_format,    SPA_POD_Id(defformat),
                SPA_FORMAT_VIDEO_size,      SPA_POD_Rectangle(&size),
                SPA_FORMAT_VIDEO_framerate, SPA_POD_Fraction(&framerate));

    pw_stream_update_params(m_stream, params, 1);
  } while(0);

  //::post("%s:%d@%s", __FILE__, __LINE__, __FUNCTION__);
  pw_thread_loop_signal (s_loop, false);
  return true;
}


/////////////////////////////////////////////////////////
// output to stream
//
/////////////////////////////////////////////////////////
bool recordPIPEWIRE :: write(imageStruct*img)
{
  if(!s_loop) {
    return false;
  }

  bool restart = false;
  /* re-initialize the stream-caps is needed */
  if(false
      || (m_image.xsize != img->xsize)
      || (m_image.ysize != img->ysize)
      || (m_image.csize != img->csize)
    ) {
    restart = true;
  }
  //::post("%s:%d@%s", __FILE__, __LINE__, __FUNCTION__);
  if(restart) {
    m_mutex.lock();
    img->copy2ImageStruct(&m_image);
    m_image.reallocate();
    m_mutex.unlock();

    pw_thread_loop_lock(s_loop);
    bool success = init(img);
    pw_thread_loop_unlock(s_loop);
    if(!success) {
      return false;
    }
  }
  m_mutex.lock();
  m_image.convertFrom(img);
  m_mutex.unlock();

  return true;
}


/////////////////////////////////////////////////////////
// codecs
//
/////////////////////////////////////////////////////////

bool recordPIPEWIRE :: setCodec(const std::string &name)
{
  //::post("%s:%d@%s", __FILE__, __LINE__, __FUNCTION__);
  if(name == "pipewire") {
    return true;
  }
  pd_error(0, "[GEM:recordPIPEWIRE] unknown codec...");
  return false;
}

std::vector<std::string> recordPIPEWIRE :: getCodecs(void)
{
  std::vector<std::string> codecs;
  codecs.push_back("pipewire");
  return codecs;
}

const std::string recordPIPEWIRE :: getCodecDescription(const std::string&codecname)
{
  if(codecname == "pipewire") {
    return "PipeWire video stream";
  } else {
    pd_error(0, "[GEM:recordPIPEWIRE] cannot describe unknown codec...'%s'", codecname.c_str());
    return "(unknown codec)";
  }
}

/////////////////////////////////////////////////////////
// properties
//
/////////////////////////////////////////////////////////

bool recordPIPEWIRE :: enumProperties(gem::Properties&props)
{
  props.clear();
  return true;
}



void recordPIPEWIRE::process_cb(void*data)
{
  //::pd_error(0, "process");
  ((recordPIPEWIRE*)data)->on_process();
}
void recordPIPEWIRE::param_changed_cb(void*data, uint32_t id, const struct spa_pod *param)
{
  ::pd_error(0, "param_changed");
  ((recordPIPEWIRE*)data)->on_param_changed(id, param);
}


void recordPIPEWIRE::on_process(void)
{
  //pw_thread_loop_signal (s_loop, false);
  ::post("%s:%d@%s<", __FILE__, __LINE__, __FUNCTION__);

  struct pw_buffer *b = pw_stream_dequeue_buffer(m_stream);
  if (!b) {
    pw_log_warn("out of buffers: %m");
    return;
  }

  struct spa_buffer *buf = b->buffer;
  if (!buf->datas[0].data) {
    pw_log_warn("NULL buffers: %m");
    return;
  }

  ::post("%s:%d@%s", __FILE__, __LINE__, __FUNCTION__);
  m_mutex.lock();
  buf->datas[0].chunk->offset = 0;
  buf->datas[0].chunk->size = m_image.xsize * m_image.ysize * m_image.csize;
  buf->datas[0].chunk->offset = 0;
  memcpy(buf->datas[0].data, m_image.data, buf->datas[0].chunk->size);

  m_mutex.unlock();

  //::post("%s", __FUNCTION__);

  pw_stream_queue_buffer(m_stream, b);
  ::post("%s:%d@%s", __FILE__, __LINE__, __FUNCTION__);
}

void recordPIPEWIRE::on_param_changed(uint32_t id, const struct spa_pod *param)
{
  //::post("%s:%d@%s", __FILE__, __LINE__, __FUNCTION__);
  if (param == NULL || id != SPA_PARAM_Format) {
    return;
  }
  struct spa_video_info format;

  if (spa_format_parse(param,
                       &format.media_type,
                       &format.media_subtype) < 0) {
    return;
  }
  if (format.media_type != SPA_MEDIA_TYPE_video ||
      format.media_subtype != SPA_MEDIA_SUBTYPE_raw) {
    return;
  }

  if (spa_format_video_raw_parse(param, &format.info.raw) < 0) {
    return;
  }

  ::post("got video format:");
  ::post("  format: %d (%s)", format.info.raw.format,
         spa_debug_type_find_name(spa_type_video_format,
                                  format.info.raw.format));
  ::post("  size: %dx%d", format.info.raw.size.width,
         format.info.raw.size.height);
  ::post("  framerate: %d/%d", format.info.raw.framerate.num,
         format.info.raw.framerate.denom);
}
