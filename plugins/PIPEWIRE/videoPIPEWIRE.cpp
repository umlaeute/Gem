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
//    WARRANTIES, see the file, "LICENSE.txt"
//
/////////////////////////////////////////////////////////
#include "videoPIPEWIRE.h"
#include "plugins/PluginFactory.h"

#include <spa/param/video/format-utils.h>
#include <spa/debug/types.h>

#include "m_pd.h"

#ifdef __BIG_ENDIAN__
# define GEM_SPA_GRAY16 SPA_VIDEO_FORMAT_GRAY16_BE
#else
# define GEM_SPA_GRAY16 SPA_VIDEO_FORMAT_GRAY16_LE
#endif

using namespace gem::plugins;

REGISTER_VIDEOFACTORY("pipewire", videoPIPEWIRE);

namespace
{
static struct pw_thread_loop *s_loop = NULL;
static unsigned int s_loopcount = 0;

bool videoPIPEWIRE_init(void)
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
void videoPIPEWIRE_deinit(void)
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
}


videoPIPEWIRE::videoPIPEWIRE(void)
  : video()
  , m_name(std::string("pipewire"))
  , m_stream(0)
  , m_format(SPA_VIDEO_FORMAT_UNKNOWN)

{
  m_pixBlock.image.xsize = 1;
  m_pixBlock.image.ysize = 1;
  m_pixBlock.image.setFormat(GEM_RGBA);
  m_pixBlock.image.reallocate();
  m_pixBlock.image.setBlack();
  videoPIPEWIRE_init();
  m_stream_events = {
    PW_VERSION_STREAM_EVENTS,
    .param_changed = param_changed_cb,
    .process = process_cb,
  };
}

videoPIPEWIRE::~videoPIPEWIRE(void)
{
  close();
  videoPIPEWIRE_deinit();
}

bool videoPIPEWIRE::open(gem::Properties&props)
{
  if(!s_loop) {
    return false;
  }

  spa_video_format defformat = SPA_VIDEO_FORMAT_RGBA;
  uint32_t width = 320;
  uint32_t height = 240;

  int flags = PW_STREAM_FLAG_NONE;
  flags |= PW_STREAM_FLAG_INACTIVE;
  flags |= PW_STREAM_FLAG_MAP_BUFFERS;

  struct pw_properties *pwprops = pw_properties_new(PW_KEY_MEDIA_TYPE, "Video",
                                  PW_KEY_MEDIA_CATEGORY, "Capture",
                                  PW_KEY_MEDIA_ROLE, "Camera",
                                  PW_KEY_APP_NAME, "Pd",
                                  PW_KEY_APP_ID, "at.iem.gem",
                                  PW_KEY_NODE_NAME, "Gem",
                                  NULL);

  std::vector<std::string>keys=props.keys();
  for(int i=0; i<keys.size(); i++) {
    const std::string key =keys[i];
    std::string s;
    double d;
    if (0) ;
    else if (("MediaRole" == key) && (props.get(key, s)) && !s.empty()) {
      pw_properties_set(pwprops, PW_KEY_MEDIA_ROLE, s.c_str());
    } else if (("AppName" == key) && (props.get(key, s))) {
      pw_properties_set(pwprops, PW_KEY_APP_NAME, s.empty()?nullptr:s.c_str());
    } else if (("NodeName" == key) && (props.get(key, s))) {
      pw_properties_set(pwprops, PW_KEY_NODE_NAME, s.empty()?nullptr:s.c_str());
#if 0
    } else if (("PortName" == key) && (props.get(key, s)) && !s.empty()) {
      pw_properties_set(pwprops, PW_KEY_PORT_NAME, s.c_str());
#endif
    } else if (("width" == key) && (props.get(key, d))) {
      width=(int)d;
    } else if (("height" == key) && (props.get(key, d))) {
      height=(int)d;
    } else if (("autoconnect" == key) && (props.get(key, d))) {
      if((int)d) {
        flags |= PW_STREAM_FLAG_AUTOCONNECT;
      }
    }
  }

  uint8_t buffer[1024];
  const struct spa_pod *params[1];
  struct spa_pod_builder b = SPA_POD_BUILDER_INIT(buffer, sizeof(buffer));
  struct spa_rectangle defsize = SPA_RECTANGLE(width, height);
  struct spa_rectangle maxsize = SPA_RECTANGLE(4096, 4096);
  struct spa_rectangle minsize = SPA_RECTANGLE(1,1);
  struct spa_fraction defrate  = SPA_FRACTION(25, 1);
  struct spa_fraction maxrate  = SPA_FRACTION(1000, 1);
  struct spa_fraction minrate  = SPA_FRACTION(0, 1);

  pw_thread_loop_lock(s_loop);
  m_stream = pw_stream_new_simple(
               pw_thread_loop_get_loop(s_loop),
               "gem-capture",
               pwprops,
               &m_stream_events,
               this);
  if(!m_stream) {
    goto err;
  }

  params[0] = (const spa_pod*)spa_pod_builder_add_object(&b,
              SPA_TYPE_OBJECT_Format, SPA_PARAM_EnumFormat,
              SPA_FORMAT_mediaType, SPA_POD_Id(SPA_MEDIA_TYPE_video),
              SPA_FORMAT_mediaSubtype, SPA_POD_Id(SPA_MEDIA_SUBTYPE_raw),
              SPA_FORMAT_VIDEO_format, SPA_POD_CHOICE_ENUM_Id(
                10
                , defformat
                , SPA_VIDEO_FORMAT_RGB
                , SPA_VIDEO_FORMAT_RGBA
                , SPA_VIDEO_FORMAT_BGR
                , SPA_VIDEO_FORMAT_BGRA
                , SPA_VIDEO_FORMAT_RGB16
                , SPA_VIDEO_FORMAT_YUY2
                , SPA_VIDEO_FORMAT_UYVY
                , SPA_VIDEO_FORMAT_GRAY8
                , GEM_SPA_GRAY16
              ),
              SPA_FORMAT_VIDEO_size, SPA_POD_CHOICE_RANGE_Rectangle(
                &defsize, &minsize, &maxsize),
              SPA_FORMAT_VIDEO_framerate, SPA_POD_CHOICE_RANGE_Fraction(
                &defrate, &minrate, &maxrate));

  pw_stream_connect(m_stream,
                    PW_DIRECTION_INPUT,
                    PW_ID_ANY,
                    (pw_stream_flags)flags,
                    params, 1);
  setProperties(props);
  pw_thread_loop_unlock(s_loop);
  return (true);
err:
  pw_thread_loop_unlock(s_loop);
  return false;
}

void videoPIPEWIRE::close()
{
  if(!m_stream) {
    return;
  }
  stop();
  pw_stream_disconnect(m_stream);
  pw_stream_destroy(m_stream);
}


bool videoPIPEWIRE::start()
{
  if(!m_stream) {
    return (false);
  }
  struct timespec abstime;
  const char*error=0;
  pw_thread_loop_lock (s_loop);
  pw_stream_set_active(m_stream, true);
  pw_thread_loop_get_time (s_loop, &abstime,
                           3 * SPA_NSEC_PER_SEC);
  /* don't wait until somebody is connected...*/
  while (false) {
    enum pw_stream_state state;
    state = pw_stream_get_state (m_stream, &error);
    if (state >= PW_STREAM_STATE_PAUSED) {
      break;
    }

    if (state == PW_STREAM_STATE_ERROR) {
      goto start_error;
    }

    if (pw_thread_loop_timed_wait_full (s_loop, &abstime) < 0) {
      error = "timeout";
      goto start_error;
    }
  }
  pw_thread_loop_signal (s_loop, false);
  pw_thread_loop_unlock (s_loop);
  return (true);
start_error: {
    ::pd_error(0, "ERROR: %s", error);
    pw_thread_loop_unlock (s_loop);
    return false;
  }
}
bool videoPIPEWIRE::stop()
{
  if(!m_stream) {
    return (false);
  }
  pw_stream_set_active(m_stream, false);
  return (true);
}



pixBlock*videoPIPEWIRE::getFrame(void)
{
  m_mutex.lock();
  if(!m_pixBlock.image.csize) {
    m_mutex.unlock();
    return 0;
  }
  return &m_pixBlock;

  m_pixBlock.image.setFormat(GEM_RGBA);
  m_pixBlock.image.reallocate();
  const unsigned int count = m_pixBlock.image.xsize * m_pixBlock.image.ysize;
  unsigned int i=0;
  unsigned char*data=m_pixBlock.image.data;
  //::post("%s", __FUNCTION__);
  m_pixBlock.newimage = true;
  pw_thread_loop_lock (s_loop);
  pw_thread_loop_signal (s_loop, false);
  pw_thread_loop_unlock (s_loop);

  return &m_pixBlock;
}
void videoPIPEWIRE::releaseFrame(void)
{
  m_pixBlock.newimage = false;
  m_mutex.unlock();
}

std::vector<std::string>videoPIPEWIRE::enumerate(void)
{
  std::vector<std::string>result;
  result.push_back("pipewire");
  return result;
}

bool videoPIPEWIRE::setDevice(int ID)
{
  return true;
}
bool videoPIPEWIRE::setDevice(const std::string&device)
{
  return ("pipewire"==device);
}
bool videoPIPEWIRE::enumProperties(gem::Properties&readable,
                                   gem::Properties&writeable)
{
  readable.clear();
  writeable.clear();
#define RWSET(id, value) writeable.set(id, value); readable.set(id, value)
  RWSET("width", 64);
  RWSET("height", 64);
  RWSET("MediaRole", std::string("Camera"));
  RWSET("AppName", std::string("Pd"));
  RWSET("NodeName", std::string("Gem"));
  //writeable.set("PortName", std::string("port")); // what is the effect of the PortName?
  writeable.set("autoconnect", 0);
  readable.set("NodeID", 0);

  return true;
}
void videoPIPEWIRE::setProperties(gem::Properties&props)
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
void videoPIPEWIRE::getProperties(gem::Properties&props)
{

  const struct pw_properties* pwprops = 0;
  if(m_stream) {
    pwprops = pw_stream_get_properties(m_stream);
#if 0
    const struct spa_dict_item *item;
    spa_dict_for_each(item, &pwprops->dict) {
      ::post("\t\t%s: \"%s\"", item->key, item->value);
    }
#endif
  }
  std::vector<std::string>keys=props.keys();
  for(unsigned int i=0; i<keys.size(); i++) {
    const std::string key =keys[i];

    if (false) {
      ;
    } else if("width"==key) {
      props.set(key, m_pixBlock.image.xsize);
    } else if("height"==key) {
      props.set(key, m_pixBlock.image.ysize);
    }

    if(m_stream) {
      if (false) {
        ;
      } else if("NodeID"==key) {
        props.set(key, pw_stream_get_node_id(m_stream));
      }
    }

    if(pwprops) {
      const char*s = 0;
      if (false) {
        ;
      } else if("MediaRole"==key && ((s = pw_properties_get(pwprops, PW_KEY_MEDIA_ROLE)))) {
        props.set(key, std::string(s));
      } else if("AppName"==key && ((s = pw_properties_get(pwprops, PW_KEY_APP_NAME)))) {
        props.set(key, std::string(s));
      } else if("NodeName"==key && ((s = pw_properties_get(pwprops, PW_KEY_NODE_NAME)))) {
        props.set(key, std::string(s));
      } else if("ClientID"==key && ((s = pw_properties_get(pwprops, PW_KEY_CLIENT_ID)))) {
        props.set(key, std::string(s));
      }
    }
  }
}

std::vector<std::string>videoPIPEWIRE::dialogs(void)
{
  std::vector<std::string>result;
  return result;
}
bool videoPIPEWIRE::provides(const std::string&name)
{
  return (name==m_name);
}
std::vector<std::string>videoPIPEWIRE::provides(void)
{
  std::vector<std::string>result;
  result.push_back(m_name);
  result.push_back("pw");
  return result;
}
const std::string videoPIPEWIRE::getName(void)
{
  return m_name;
}

void videoPIPEWIRE::on_process(void)
{
  //pw_thread_loop_signal (s_loop, false);
  struct pw_buffer *b;
  struct spa_buffer *buf;

  if ((b = pw_stream_dequeue_buffer(m_stream)) == NULL) {
    pw_log_warn("out of buffers: %m");
    return;
  }

  buf = b->buffer;
  if (buf->datas[0].data == NULL) {
    return;
  }

  m_mutex.lock();
  m_pixBlock.image.not_owned = false;
  switch(m_format) {
  case SPA_VIDEO_FORMAT_RGB:
    m_pixBlock.image.fromRGB((unsigned char*)buf->datas[0].data);
    break;
  case SPA_VIDEO_FORMAT_RGBA:
    m_pixBlock.image.fromRGBA((unsigned char*)buf->datas[0].data);
    break;
  case SPA_VIDEO_FORMAT_BGR:
    m_pixBlock.image.fromBGR((unsigned char*)buf->datas[0].data);
    break;
  case SPA_VIDEO_FORMAT_BGRA:
    m_pixBlock.image.fromBGRA((unsigned char*)buf->datas[0].data);
    break;
  case SPA_VIDEO_FORMAT_RGB16:
    m_pixBlock.image.fromRGB16((unsigned char*)buf->datas[0].data);
    break;
  case SPA_VIDEO_FORMAT_YUY2:
    m_pixBlock.image.fromYUY2((unsigned char*)buf->datas[0].data);
    break;
  case SPA_VIDEO_FORMAT_GRAY8:
    m_pixBlock.image.fromGray((unsigned char*)buf->datas[0].data);
    break;
  case GEM_SPA_GRAY16:
    m_pixBlock.image.fromGray((short*)buf->datas[0].data);
    break;
  default:
    m_pixBlock.image.data = (unsigned char*)buf->datas[0].data;
    m_pixBlock.image.not_owned = true;
  }

  m_pixBlock.newimage = true;
  m_mutex.unlock();

  pw_stream_queue_buffer(m_stream, b);
}

void videoPIPEWIRE::on_param_changed(uint32_t id, const struct spa_pod *param)
{
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


  m_mutex.lock();
  m_pixBlock.newfilm = true;
  m_pixBlock.image.xsize = format.info.raw.size.width;
  m_pixBlock.image.ysize = format.info.raw.size.height;
  m_format = format.info.raw.format;
  switch(format.info.raw.format) {
  case(SPA_VIDEO_FORMAT_RGB):
  case(SPA_VIDEO_FORMAT_RGBA):
  case(SPA_VIDEO_FORMAT_BGR):
  case(SPA_VIDEO_FORMAT_BGRA):
  case(SPA_VIDEO_FORMAT_RGB16):
    m_pixBlock.image.setFormat(GEM_RGBA);
    break;
  case(SPA_VIDEO_FORMAT_YUY2):
  case(SPA_VIDEO_FORMAT_UYVY):
    m_pixBlock.image.setFormat(GEM_YUV);
    break;
  case(SPA_VIDEO_FORMAT_GRAY8):
  case(SPA_VIDEO_FORMAT_GRAY16_BE):
  case(SPA_VIDEO_FORMAT_GRAY16_LE):
    m_pixBlock.image.setFormat(GEM_GRAY);
    break;
  default:
    m_pixBlock.image.csize = 0;
    break;
  }
  if(m_pixBlock.image.csize) {
    m_pixBlock.image.reallocate();
  } else {
    m_pixBlock.image.reallocate(m_pixBlock.image.xsize * m_pixBlock.image.ysize * 4);
  }
  m_mutex.unlock();
  //return;

  ::post("got video format:");
  ::post("  format: %d (%s)", format.info.raw.format,
         spa_debug_type_find_name(spa_type_video_format,
                                  format.info.raw.format));
  ::post("  size: %dx%d", format.info.raw.size.width,
         format.info.raw.size.height);
  ::post("  framerate: %d/%d", format.info.raw.framerate.num,
         format.info.raw.framerate.denom);

}

void videoPIPEWIRE::process_cb(void*data)
{
  //::pd_error(0, "process");
  ((videoPIPEWIRE*)data)->on_process();
}
void videoPIPEWIRE::param_changed_cb(void*data, uint32_t id, const struct spa_pod *param)
{
  //::pd_error(0, "param_changed");
  ((videoPIPEWIRE*)data)->on_param_changed(id, param);
}
