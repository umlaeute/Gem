////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2001-2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#if defined HAVE_LIBUNICAP && !defined HAVE_UNICAP
# define HAVE_UNICAP
#endif


#ifdef HAVE_UNICAP

#include "videoUNICAP.h"
#include "plugins/PluginFactory.h"

using namespace gem::plugins;

// for debugging...
#include "Gem/RTE.h"

/* debugging helpers  */
#ifdef debugPost
# undef debugPost
#endif
#ifdef debugThread
# undef debugThread
#endif

#if 0
# define debugPost ::startpost("%s:%s[%d]", __FILE__, __FUNCTION__, __LINE__), ::post
#else
# include "Utils/nop.h"
# define debugPost nop_post
#endif

#if 0
# define debugThread ::startpost("%s:%s[%d]", __FILE__, __FUNCTION__, __LINE__), ::post
#else
# include "Utils/nop.h"
# define debugThread nop_post
#endif


/////////////////////////////////////////////////////////
//
// videoUNICAP
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

#include <sys/stat.h>
#include <math.h>

REGISTER_VIDEOFACTORY("unicap", videoUNICAP);


namespace
{
static const std::string s_name = std::string("unicap");
}

videoUNICAP :: videoUNICAP(void) :
  m_width(-1), m_height(-1),
  m_handle(NULL),
  m_devicenum(-1),
  m_reqFormat(GEM_RGBA),
  m_running(false)
{
  enumerate();
}

////////////////////////////////////////////////////////
// Destructor
//
////////////////////////////////////////////////////////
videoUNICAP :: ~videoUNICAP(void)
{
  close();
}

//////////////////
// this reads the data that was captured by capturing() and returns it within a pixBlock
pixBlock *videoUNICAP :: getFrame(void)
{
  mutex.lock();
  return &m_pix;
}

void videoUNICAP::releaseFrame(void)
{
  mutex.unlock();
}

bool videoUNICAP :: open(gem::Properties&props)
{
  std::vector<unsigned int>ds;
  if(m_devicename.empty()) {
    if(m_devicenum>=0) {
      ds.push_back(m_devicenum);
    } else {
      ds.push_back(0);
    }
  } else {
    ds=m_name2devices[m_devicename];
  }
  unicap_status_t status = -1;
  for(int i=0; i<ds.size(); i++) {
    unicap_device_t device;
    unsigned int d=ds[i];
    status = unicap_enumerate_devices (NULL, &device, d); // (1)
    if(SUCCESS(status)) {
      status = unicap_open (&m_handle, &device);
      if(SUCCESS(status)) {
        continue;
      }
    }
  }
  if(!SUCCESS(status)) {
    return false;
  }

  setProperties(props);
  return true;
}
void videoUNICAP :: close(void)
{
  if(m_handle) {
    unicap_close(m_handle);
    m_handle=NULL;
  }
}

void videoUNICAP::newFrameCB (unicap_event_t event,
                              unicap_handle_t handle,
                              unicap_data_buffer_t * buffer,
                              void *usr_data)
{
  videoUNICAP*v=(videoUNICAP*)usr_data;
  debugPost("newFrameCB");
  if(v && UNICAP_EVENT_NEW_FRAME==event) {
    v->newFrame(handle, buffer);
  }
}

namespace
{
typedef enum {
  FIRST=0,
  RGB,
  RGBA,
  BGR,
  BGRA,
  RGB16,
  ABGR,
  ARGB,
  GREY,
  GREY16,
  UYVY,
  YUY2,
  YVYU,
  YV12,
  YU12,

  ILLEGAL
} fourcc_t;

#ifdef FOURCC
# undef FOURCC
#endif


#define FOURCC(a,b,c,d) (unsigned int)((((unsigned int)d)<<24)+(((unsigned int)c)<<16)+(((unsigned int)b)<<8)+a)
/*
  #define FOURCC(a) (unsigned int) (
  ((((unsigned char*)a)[0])<<24)+             \
  ((((unsigned char*)a)[1])<<16)+             \
  ((((unsigned char*)a)[2])<< 8)+             \
  ((((unsigned char*)a)[3])<< 0))
*/

static void post_fmt(unicap_format_t*fmt)
{
  if(!fmt) {
    return;
  }
  const char*buftype="unknown";
  if(UNICAP_BUFFER_TYPE_USER==fmt->buffer_type) {
    buftype="USER";
  }
  if(UNICAP_BUFFER_TYPE_SYSTEM==fmt->buffer_type) {
    buftype="SYSTEM";
  }

  debugPost("format '%s': ((%dx%d+%d+%d)) < (%dx%d+%d+%d) < ((%dx%d+%d+%d)) in +%d+%d steps\n"
            "\t%d sizes\n"
            "\tbuffers: %d types, %d systembuffers, %d size, type=%s"
            "",
            fmt->identifier,
            fmt->min_size.width, fmt->min_size.height,fmt->min_size.x, fmt->min_size.y,
            fmt->size.width, fmt->size.height,fmt->size.x, fmt->size.y,
            fmt->max_size.width, fmt->max_size.height,fmt->max_size.x, fmt->max_size.y,
            fmt->h_stepping, fmt->v_stepping,

            fmt->size_count,

            fmt->buffer_types, fmt->system_buffer_count, fmt->buffer_size, buftype,
            0);
}

fourcc_t fourcc2fmt(unsigned int fourcc)
{
  fourcc_t format=ILLEGAL;
  switch(fourcc) {
  default:
    format=ILLEGAL;
    break;

  case (FOURCC('R', 'G', 'B', '2')):
    format=RGB;
    break;
  case (FOURCC('R', 'G', 'B', ' ')):
    format=RGB;
    break;
  case (FOURCC('R', 'G', 'B', 'A')):
    format=RGBA;
    break;
  case (FOURCC('B', 'G', 'R', ' ')):
    format=BGR;
    break;
  case (FOURCC('B', 'G', 'R', 'A')):
    format=BGRA;
    break;
  case (FOURCC('A', 'B', 'G', 'R')):
    format=ABGR;
    break;
  case (FOURCC('A', 'R', 'G', 'B')):
    format=ARGB;
    break;

  case (FOURCC('Y', '8', '0', '0')):
  case (FOURCC('Y', ' ', ' ', ' ')):
  case (FOURCC('G', 'R', 'E', 'Y')):
    format=GREY;
    break;
  case (FOURCC('Y', '1', '6', ' ')):
    format=GREY16;
    break;

  case (FOURCC('U', 'Y', 'N', 'V')):
  case (FOURCC('Y', '4', '2', '2')):
  case (FOURCC('H', 'D', 'Y', 'C')): // ?
  case (FOURCC('U', 'Y', 'V', 'Y')):
    format=UYVY;
    break;

  case (FOURCC('Y', 'U', 'Y', 'V')):
  case (FOURCC('Y', 'U', 'N', 'V')):
  case (FOURCC('Y', 'U', 'Y', '2')):
    format=YUY2;
    break;

  case (FOURCC('Y', 'V', 'Y', 'U')):
    format=YVYU;
    break;
  }

  return format;
}

float dimension_penalty(int w, int h, unicap_rect_t&size)
{
  if(w>0 && h>0) {
    if(1) {
      // compare diagonals (penalty for very stretched formats
      double r=sqrt(w*w+h*h);
      double r0=sqrt(size.width*size.width+size.height*size.height);
      return fabs(r-r0);
    } else {
      // compare area
      double r=sqrt(w*h);
      double r0=sqrt(size.width*size.height);
      return fabs(r-r0);
    }
  } else if (w>0) {
    return fabs(w-size.width);
  } else if (h>0) {
    return fabs(h-size.height);
  }
  return 0.;
}


};


void videoUNICAP::newFrame (unicap_handle_t handle,
                            unicap_data_buffer_t * buffer)
{
  unicap_format_t*fmt=&(buffer->format);
  post_fmt(fmt);
  fourcc_t format=fourcc2fmt(fmt->fourcc);

  if(ILLEGAL==format) {
    logpost(0, 3+1, "[GEM:videoUNICAP] unsupported format '%s'", fmt->identifier);
    return;
  }

  unsigned char*data=buffer->data;
  debugPost("data: %p", data);

  mutex.lock();
  m_pix.image.xsize=fmt->size.width;
  m_pix.image.ysize=fmt->size.height;

  m_pix.image.upsidedown=true;

  m_pix.image.reallocate(); // actually this is also done in from*()
  switch (format) {
  case RGB:
    m_pix.image.fromRGB(data);
    break;
  case RGBA:
    m_pix.image.fromRGBA(data);
    break;
  case BGR:
    m_pix.image.fromBGR(data);
    break;
  case BGRA:
    m_pix.image.fromBGRA(data);
    break;
  case RGB16:
    m_pix.image.fromRGB16(data);
    break;
  case ABGR:
    m_pix.image.fromABGR(data);
    break;
  case ARGB:
    m_pix.image.fromARGB(data);
    break;
  case GREY:
    m_pix.image.fromGray(data);
    break;
  case GREY16:
    m_pix.image.fromGray((short*)data);
    break;
  case UYVY:
    m_pix.image.fromUYVY(data);
    break;
  case YUY2:
    m_pix.image.fromYUY2(data);
    break;
  case YVYU:
    m_pix.image.fromYVYU(data);
    break;
  case YV12:
    m_pix.image.fromYV12(data);
    break;
  case YU12:
    m_pix.image.fromYU12(data);
    break;
  default:
    logpost(0, 3+1, "[GEM:videoUNICAP] cannot convert from given format");
    break;
  }
  m_pix.newimage=1;

  mutex.unlock();
}


/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
bool videoUNICAP :: start(void)
{
  if(m_running) {
    stop();
  }
  unicap_status_t status = 0;
  unicap_format_t format;
  //  defaultFormat();
  unicap_format_t format_spec;
  unicap_void_format( &format_spec );

  int default_format=-1;
  int default_size=0;

  unsigned int count_format=0;

  std::string s;
  if(m_props.get("format", s)) {
    for(unsigned i = 0;
        SUCCESS( unicap_enumerate_formats( m_handle, NULL, &format, i ) ); i++) {
      count_format++;
      if(s == format.identifier) {
        default_format=i;
        break;
      }
    }
  }

  std::vector<int>formatid;
  if(default_format>=0) {
    formatid.push_back(default_format);
  } else {
    for(int i=0; i<count_format; i++) {
      formatid.push_back(i);
    }
  }

  int w=-1;
  int h=-1;
  double d;
  if(m_props.get("width", d) && d>0) {
    w=d;
  }
  if(m_props.get("height", d) && d>0) {
    h=d;
  }

  double penalty=-1.;
  debugPost("dimensions: %dx%d\t%f", w, h, penalty);
  if ((w>0 || h>0)) {
    /* try to find a format that matches the desired width/height best */
    for(unsigned int formatid_index=0; formatid_index<formatid.size();
        formatid_index++) {
      int format_index=formatid[formatid_index];
      if( !SUCCESS( unicap_enumerate_formats( m_handle, &format_spec, &format,
                                              format_index) ) )  {
        logpost(0, 3+1, "[GEM:videoUNICAP] Failed to get video format %d",
                format_index);
        continue;
      }

      /* If a video format has more than one size, try to find the best match */
      if(format.size_count) {
        if(penalty<0) {
          penalty=dimension_penalty(w, h, format.sizes[0]);
          default_format=format_index;
          default_size=0;
        }
        debugPost("current penalty=%f", penalty);
        for( unsigned size_index = 0; size_index < format.size_count;
             size_index++ ) {
          double p=dimension_penalty(w, h, format.sizes[size_index]);
          debugPost("[%d/%d] penalty for (%dx%d) vs (%dx%d)=%f <> %f", format_index,
                    size_index,
                    w, h, format.sizes[size_index].width, format.sizes[size_index].height,
                    p, penalty);
          if(p<penalty) {
            penalty=p;
            default_format=format_index;
            default_size=size_index;
            debugPost("new champion format=%d\tsize=%d", default_format, default_size);
          }
        }
      }

    }
  }


  unicap_void_format( &format_spec );
  if( !SUCCESS( unicap_enumerate_formats( m_handle, &format_spec, &format,
                                          default_format) ) )  {
    return false;
  }
  post_fmt(&format);
  if(format.size_count<1 || format.sizes==NULL) {
    pd_error(0, "[GEM:videoUNICAP] can't get supported formats. Device might be busy.");
    return false;
  }

  if(default_size>format.size_count) {
    debugPost("oops: want size #%d of %d", default_size, format.size_count);
    default_size=0;
  }
  format.size.width = format.sizes[default_size].width;
  format.size.height = format.sizes[default_size].height;

  format.buffer_type = UNICAP_BUFFER_TYPE_SYSTEM;
  debugPost("setting format (%d/%d)", default_format, default_size);
  post_fmt(&format);
  if( !SUCCESS( unicap_set_format( m_handle, &format ) ) )  {
    pd_error(0, "[GEM:videoUNICAP]failed to set format");
    return false;
  }
  unicap_unregister_callback(m_handle, UNICAP_EVENT_NEW_FRAME);
  status=unicap_register_callback (m_handle,
                                   UNICAP_EVENT_NEW_FRAME,
                                   (unicap_callback_t) newFrameCB,
                                   (void *) this);
  debugPost("registered callback: %x", status);
  if(!SUCCESS(status)) {
    debugPost("callback miserably failed");
    return false;
  }
  status=unicap_start_capture (m_handle);
  debugPost("start capture: %x", status);
  if(!SUCCESS(status)) {
    return false;
  }

  m_running=true;
  return m_running;
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
bool videoUNICAP :: stop(void)
{
  bool wasrunning=m_running;
  if(m_running) {
    unicap_stop_capture (m_handle);  // (3)
  }

  m_running=false;

  return wasrunning;
}

bool videoUNICAP :: reset(void)
{
#warning reset
  bool wasrunning=stop();
  close();
  enumerate();
  if(wasrunning) {
    open(m_props);
    start();
  }
  return true;
}

bool videoUNICAP :: setColor(int format)
{
  if (format<=0 || format==m_reqFormat) {
    return false;
  }
  m_reqFormat=format;
  //  restartTransfer();
  return true;
}

std::vector<std::string> videoUNICAP::enumerate(void)
{
  std::vector<std::string> result;
  int devcount=0;
  unicap_status_t status = 0;

  m_providers.clear();
  m_providers.push_back(s_name);
  m_providers.push_back("analog");


  status = unicap_reenumerate_devices(&devcount);
  if(!SUCCESS(status)) {
    return result;
  }

  m_devices.clear();
  m_name2devices.clear();


  for(int i=0; i<devcount; i++) {
    unicap_device_t device;
    status = unicap_enumerate_devices (NULL, &device, i); // (1)


    if(SUCCESS(status)) {
      const unsigned int cur=m_devices.size();
#if 0
      logpost(0, 3+1,
              "[GEM:videoUNICAP] ID='%s'\tmodel='%s'\tvendor='%s'\tdevice='%s'\tCPI='%s'",
              device.identifier,
              device.model_name,
              device.vendor_name,
              device.device,
              device.cpi_layer);
#endif
      m_devices.push_back(device);

      m_name2devices[device.identifier ].push_back(cur);
      m_name2devices[device.model_name ].push_back(cur);
      m_name2devices[device.vendor_name].push_back(cur);
      if(device.device[0]) {
        m_name2devices[device.device].push_back(cur);
      }

      /* hmm, think about this .... */
      std::string cpi=device.cpi_layer;
      cpi=cpi.substr(cpi.rfind("/")+1);
      cpi=cpi.substr(0, cpi.find("."));

      provide(cpi);

      result.push_back(device.identifier);
    }
  }

  return result;
}
bool videoUNICAP::setDevice(int ID)
{
  m_devicename.clear();
  m_devicenum=ID;
  return true;
}
bool videoUNICAP::setDevice(const std::string&ID)
{
  m_devicename=ID;
  m_devicenum=-1;
  return true;
}

bool videoUNICAP :: defaultFormat(void)
{
  if(!m_handle) {
    return false;
  }
  int count=0;
  unicap_status_t status= unicap_reenumerate_formats(m_handle, &count );
  if(!SUCCESS(status)) {
    return false;
  }

  debugPost("got %d formats", count);
  if(count==0) {
    return true;
  }

  unicap_format_t format;

  for(int i=0; i<count; i++) {

    status=unicap_enumerate_formats( m_handle, NULL, &format, i );

    post_fmt(&format);

#if 0
    if(count==1) {

      status= unicap_set_format(m_handle, &format);
      return SUCCESS(status);
    }
#endif
  }

  return true;
}


bool videoUNICAP :: enumProperties(gem::Properties&readable,
                                   gem::Properties&writeable)
{
  readable.clear();
  writeable.clear();

  if(m_handle) {
    int count=0;
    unicap_status_t status= unicap_reenumerate_properties(m_handle, &count );
    if(!SUCCESS(status)) {
      return false;
    }

    for(int id=0; id<count; id++) {
      unicap_property_t prop;
      gem::any typ;

      status = unicap_enumerate_properties(m_handle, NULL, &prop, id);
      if(!SUCCESS(status)) {
        continue;
      }

      debugPost("id='%s'\tcat='%s'\tunit='%s'\tflags=%d",
                prop.identifier,
                prop.category,
                prop.unit,
                prop.flags);


      switch(prop.type) {
      case UNICAP_PROPERTY_TYPE_RANGE:
        debugPost("range %f-%f", prop.range.min, prop.range.min);
        typ=prop.range.max;
        break;
      case UNICAP_PROPERTY_TYPE_VALUE_LIST:
        debugPost("value_list %d", prop.value_list.value_count);
        typ=prop.value_list.value_count;
        break;
      case UNICAP_PROPERTY_TYPE_MENU:
        debugPost("menu '%s' of %d", prop.menu_item, prop.menu.menu_item_count);
        typ=std::string(prop.menu_item);//prop.menu.menu_item_count;
        break;
      case UNICAP_PROPERTY_TYPE_FLAGS:
        debugPost("flags");
        break;
      default:
        debugPost("unknown");
        // ?
        break;
      }

      readable.set(prop.identifier, typ);
      if(!(prop.flags & UNICAP_FLAGS_READ_ONLY)) {
        writeable.set(prop.identifier, typ);
      }

#warning check UNICAP_FLAGS_ON_OFF & UNICAP_FLAGS_ONE_PUSH
    }
  }
  return true;
}
void videoUNICAP :: getProperties(gem::Properties&props)
{
  if(!m_handle) {
    return;
  }
  unicap_status_t status=0;

  std::vector<std::string> keys=props.keys();

  bool getwidth=false, getheight=false;
  for(int i=0; i<keys.size(); i++) {
    std::string key=keys[i];
    unicap_property_t prop;
    strncpy(prop.identifier, key.c_str(), 127);
    prop.identifier[127]=0;

    if("width"==key) {
      getwidth=true;
      continue;
    }
    if("height"==key) {
      getheight=true;
      continue;
    }

    status= unicap_get_property(m_handle, &prop );

    if(SUCCESS(status)) {
      switch(prop.type) {
      case UNICAP_PROPERTY_TYPE_VALUE_LIST:
      case UNICAP_PROPERTY_TYPE_FLAGS:
      case UNICAP_PROPERTY_TYPE_RANGE:
        props.set(key, prop.value);
        break;
      case UNICAP_PROPERTY_TYPE_MENU:
        props.set(key, std::string(prop.menu_item));
        break;
      default:
        props.erase(key);
        // ?
        break;
      }
    }
  }


  if(getwidth||getheight) {
    unicap_format_t fmt;
    status=unicap_get_format(m_handle, &fmt);
    post("getting dimen");
    post_fmt(&fmt);
    if(SUCCESS(status)) {
      if(getwidth ) {
        props.set("width", fmt.size.width);
      }
      if(getheight) {
        props.set("height", fmt.size.height);
      }
    }
  }


}
void videoUNICAP :: setProperties(gem::Properties&props)
{
  m_props=props;
  debugPost("handle=%p", m_handle);
  if(!m_handle) {
    return;
  }

  unicap_status_t status = 0;
  bool restart=false;

  unsigned int width=0, height=0;

  std::vector<std::string> keys=props.keys();
  for(int i=0; i<keys.size(); i++) {
    std::string key=keys[i];

    double d=0;
    std::string s;


    if(("width"==key) && props.get(key, d)) {
      width=d;
      if(m_width!=width) {
        m_width=width;
        restart=true;
      }
      continue;
    }
    if(("height"==key) && props.get(key, d)) {
      height=d;
      if(m_height!=height) {
        m_height=height;
        restart=true;
      }
      continue;
    }

    unicap_property_t prop;
    strncpy(prop.identifier, key.c_str(), 127);
    prop.identifier[127]=0;
    status=unicap_get_property(m_handle, &prop );

    if(SUCCESS(status)) {
      switch(prop.type) {
      case UNICAP_PROPERTY_TYPE_VALUE_LIST:
      case UNICAP_PROPERTY_TYPE_FLAGS:
      case UNICAP_PROPERTY_TYPE_RANGE:
        if(props.get(key, d)) {
          prop.value=d;
          status= unicap_set_property(m_handle, &prop );
        }
        break;
      case UNICAP_PROPERTY_TYPE_MENU:
        if(props.get(key, d)) {
          if(d>=0 && d < prop.menu.menu_item_count) {
            int i=d;
            /* unfortunately we must use the symbolic value and cannot simply set using the index... */
            strncpy(prop.menu_item, prop.menu.menu_items[i], 127);
            prop.menu_item[127]=0;
            status= unicap_set_property(m_handle, &prop );
          }
        } else if (props.get(key, s)) {
          strncpy(prop.menu_item, s.c_str(), 127);
          prop.menu_item[127]=0;
          status= unicap_set_property(m_handle, &prop );
        }
        break;
      default:
        // ?
        break;
      }

      if(!SUCCESS(status)) {
        logpost(0, 3+1, "[GEM:videoUNICAP] could not set property '%s'", key.c_str());
#if 0
      } else {
        logpost(0, 3+1, "[GEM:videoUNICAP] successfully set property '%s'",
                key.c_str());
#endif
      }
    }
  }

  while(restart) {
    restart=false;
    debugPost("restarting stream due to property change");
    bool running=stop();
    debugPost("running=%d", running);
    if (running) {
      start();
    }
  }
}

void videoUNICAP::provide(const std::string&name)
{
  if(!provides(name)) {
    m_providers.push_back(name);
  }
}

bool videoUNICAP::provides(const std::string&name)
{
  for(unsigned int i=0; i<m_providers.size(); i++) {
    if(name == m_providers[i]) {
      return true;
    }
  }
  return (false);
}
std::vector<std::string>videoUNICAP::provides(void)
{
  std::vector<std::string>result;
  for(unsigned int i=0; i<m_providers.size(); i++) {
    result.push_back(m_providers[i]);
  }
  return result;
}
const std::string videoUNICAP::getName(void)
{
  return s_name;
}

#endif /* HAVE_UNICAP */
