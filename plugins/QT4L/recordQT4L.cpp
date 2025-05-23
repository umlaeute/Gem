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

#include "recordQT4L.h"
#include "Gem/RTE.h"
#include "Gem/Manager.h"

#include "plugins/PluginFactory.h"


using namespace gem::plugins;

#include <stdlib.h>

#define TIMEBASE 1000000


#ifdef  GEM_USE_RECORDQT4L
#include <lqt_version.h>
REGISTER_RECORDFACTORY("quicktime4linux", recordQT4L);
#endif
/////////////////////////////////////////////////////////
//
// recordQT4L
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

recordQT4L :: recordQT4L(void) :
#if defined  GEM_USE_RECORDQT4L
  m_qtfile(NULL),
  m_codec(NULL), m_codecs(NULL),
  m_codecname(std::string()),
  m_qtbuffer(NULL),
  m_colormodel(0),
  m_width(-1), m_height(-1),
  m_restart(true),
  m_useTimeStamp(true),
  m_startTime(0.),
  m_timeTick(1.),
  m_curFrame(0)
{
  lqt_registry_init ();
  std::vector<std::string>codecs=getCodecs();
  if(codecs.size()>0) {
    setCodec(codecs[0]);
    logpost(0, 3+1, "[GEM:recordQT4L] default codec is: '%s'", m_codecname.c_str());
  }
}
#else
{}
#endif

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
recordQT4L :: ~recordQT4L(void)
{
  stop();
}

#if defined  GEM_USE_RECORDQT4L
void recordQT4L :: stop(void)
{
  if(m_qtfile) {
    quicktime_close(m_qtfile);
    m_qtfile=NULL;
  }
#if 0
  /* this crashes badly! */
  if(m_qtbuffer) {
    lqt_rows_free(m_qtbuffer);
  }
#endif
}

/////////////////////////////////////////////////////////
// open a file !
//
/////////////////////////////////////////////////////////
static struct {
  const char * name;
  const lqt_file_type_t type;
  const char * extension;
  const char * description;
  const char * default_video_codec;
} qtformats[] =  {
  { "qt",       LQT_FILE_QT,        "mov", "Quicktime (QT7 compatible)",   "yuv2" }, /* ffmpeg_mpg4 */
  { "qtold",    LQT_FILE_QT_OLD,    "mov", "Quicktime (qt4l and old lqt)", "yuv2" }, /* mjpa */
  { "avi_odml", LQT_FILE_AVI_ODML,  "avi", "AVI (> 2G)",                   "yuv2" }, /* ffmpeg_msmpeg4v3 */
  { "avi",      LQT_FILE_AVI,       "avi", "AVI (< 2G)",                   "yuv2" }, /* ffmpeg_msmpeg4v3 */
  { "mp4",      LQT_FILE_MP4,       "mp4", "ISO MPEG-4",                   "yuv2" }, /* ffmpeg_mpg4 */
  { "m4a",      LQT_FILE_M4A,       "m4a", "m4a (iTunes compatible)",      "yuv2" }, /* ffmpeg_mpg4 */
};
/* get the filetype from the short name */
static lqt_file_type_t get_qtformat(const char*name)
{
  for(unsigned int i = 0; i < sizeof(qtformats)/sizeof(*qtformats); i++) {
    if(!strcasecmp(name, qtformats[i].name)) {
      logpost(0, 3+1, "[GEM:recordQT4L] using format '%s'", qtformats[i].description);
      return qtformats[i].type;
    }
  }

  logpost(0, 3+0,
          "[GEM:recordQT4L] unknown extension: encoding will be QuickTime");
  return LQT_FILE_QT; /* should be save for now */
}
/* guess the file-format by inspecting the extension */
static lqt_file_type_t guess_qtformat(const std::string&filename)
{
  const char * extension = strrchr(filename.c_str(), '.');

  if(!extension) {
    logpost(0, 3+0, "[GEM:recordQT4L] no extension given: encoding will be QuickTime");
    return LQT_FILE_QT;
  }

  extension++;

  for(unsigned int i = 0; i < sizeof(qtformats)/sizeof(*qtformats); i++) {
    if(!strcasecmp(extension, qtformats[i].extension)) {
      logpost(0, 3+1, "[GEM:recordQT4L] detected format '%s'", qtformats[i].description);
      return qtformats[i].type;
    }
  }

  logpost(0, 3+0,
          "[GEM:recordQT4L] unknown extension: encoding will be QuickTime");
  return LQT_FILE_QT; /* should be save for now */
}

bool recordQT4L :: start(const std::string&filename, gem::Properties&props)
{
  stop();

  lqt_file_type_t format = LQT_FILE_NONE;
  std::string s;
  if (props.get("lqtformat", s)) {
    format = get_qtformat(s.c_str());
  }

  if (LQT_FILE_NONE == format) {
    format =  guess_qtformat(filename);
  }

  m_curTrack = -1;
  m_qtfile = lqt_open_write(filename.c_str(), format);
  if(m_qtfile==NULL) {
    pd_error(0, "[GEM:recordQT4L] starting to record to %s failed",
             filename.c_str());
    return false;
  }

#define prop2quicktime(qtfile, propname) \
  if (props.get(#propname, s)) { quicktime_set_##propname(qtfile, (char*)s.c_str()); }
#define prop2lqt(qtfile, propname) \
  if (props.get(#propname, s)) { lqt_set_##propname(qtfile, (char*)s.c_str()); }

  prop2quicktime(m_qtfile, name);
  prop2quicktime(m_qtfile, copyright);
  prop2quicktime(m_qtfile, info);

  prop2lqt(m_qtfile, album);
  prop2lqt(m_qtfile, artist);
  prop2lqt(m_qtfile, genre);
  prop2lqt(m_qtfile, track);
  prop2lqt(m_qtfile, comment);
  prop2lqt(m_qtfile, author);

  m_props=props;

  m_restart=true;
  return (true);
}

/////////////////////////////////////////////////////////
// initialize the encoder
//
/////////////////////////////////////////////////////////

static void applyProperties(quicktime_t*file, int track,
                            lqt_codec_info_t*codec,
                            gem::Properties&props)
{

  if(NULL==file || NULL==codec) {
    return;
  }
  std::vector<std::string>keys=props.keys();

  std::map<std::string, lqt_parameter_type_t>proptypes;
  for(int i=0; i<codec->num_encoding_parameters; i++) {
    proptypes[codec->encoding_parameters[i].name]=
      codec->encoding_parameters[i].type;
  }
  for(int i=0; i<keys.size(); i++) {
    std::string key=keys[i];
    std::map<std::string,lqt_parameter_type_t>::iterator it = proptypes.find(
          key);
    if(it!=proptypes.end()) {
      void*value=NULL;
      int v_i=0;
      float v_f=0.f;
      const char*v_s=NULL;
#if defined LQT_MAKE_BUILD && defined LQT_BUILD
# if LQT_BUILD >= LQT_MAKE_BUILD(1,0,2)
      const char* q_key = key.c_str();
# else
      char* q_key=const_cast<char*>(key.c_str());
# endif
#else
      char* q_key=const_cast<char*>(key.c_str());
#endif

      double d;
      std::string s;
      switch(proptypes[key]) {
      case LQT_PARAMETER_INT:
        if(props.get(key, d)) {
          v_i=static_cast<int>(d);
          value=static_cast<void*>(&v_i);
        }
        break;
      case LQT_PARAMETER_FLOAT:
        if(props.get(key, d)) {
          v_f=static_cast<float>(d);
          value=static_cast<void*>(&v_f);
        }
        break;
      case LQT_PARAMETER_STRING:
        if(props.get(key, s)) {
          v_s=s.c_str();
          value=static_cast<void*>(const_cast<char*>(v_s));
        }
        break;
      }
      if(value) {
        lqt_set_video_parameter(file, track, q_key, value);
      }
    }

  }

}

static int try_colormodel(quicktime_t *          file,
                          int   track,
                          int   colormodel)
{
  if(quicktime_writes_cmodel(file, colormodel, track)) {
    lqt_set_cmodel(file, track, colormodel);
    return colormodel;
  }
  return 0;
}
static int try_colormodel(quicktime_t *          file,
                          int   track,
                          std::vector<int>      colormodel)
{
  for(int i=0; i<colormodel.size(); i++) {
    int result=try_colormodel(file, track, colormodel[i]);
    if(result) {
      return result;
    }
  }
  return 0;
}


bool recordQT4L :: init(const imageStruct*img, double fps)
{
  int rowspan=0, rowspan_uv=0;
  lqt_codec_info_t*codec=NULL;
  int err=0;

  if(!m_qtfile || !img || fps < 0.) {
    return false;
  }

  /* do we have a codec specified? */
  if(NULL==m_codec) {
    setCodec(m_codecname);
  }
  if(NULL==m_codec) {
    pd_error(0, "[GEM:recordQT4L] couldn't initialize codec");
    return false;
  }

  m_useTimeStamp=true;

  double d;
  if(m_props.get(std::string("framerate"), d)) {
    if(d>0) {
      m_useTimeStamp=false;
      fps=d;
    }
  }

  m_startTime=clock_getlogicaltime();
  m_curFrame=0;
  m_timeTick=TIMEBASE/fps;

  err=lqt_add_video_track(m_qtfile,
                          img->xsize,
                          img->ysize,
                          m_timeTick,
                          TIMEBASE,
                          m_codec);
  if(err!=0) {
    return false;
  }
  m_curTrack++;

  applyProperties(m_qtfile, m_curTrack, m_codec, m_props);

  /* set the colormodel */
  std::vector<int>trycolormodels;
  trycolormodels.push_back(BC_RGBA8888);
  trycolormodels.push_back(BC_RGB888);
  trycolormodels.push_back(BC_YUV422);

  m_colormodel=try_colormodel(m_qtfile, m_curTrack, trycolormodels);
  if(!m_colormodel) {
    return false;
  }

  /* make sure to allocate enough buffer; it sometimes crashes when i allocate the "right" size,
     so we just grab a multiple of what we actually want...
  */
  /* but isn't this a memleak? it sure crashes if i try to lqt_rows_free() the qtbuffer */
  m_qtbuffer = lqt_rows_alloc(2*img->xsize, 2*img->ysize, m_colormodel,
                              &rowspan, &rowspan_uv);

  m_width =img->xsize;
  m_height=img->ysize;

  return true;
}
/////////////////////////////////////////////////////////
// do the actual encoding and writing to file
//
/////////////////////////////////////////////////////////
bool recordQT4L :: write(imageStruct*img)
{
  if(!m_qtfile || !img) {
    return false;
  }
  unsigned char**rowpointers;
  int row, row_stride;
  float framerate = GemMan::getFramerate();

  if(m_width!=img->xsize || m_height!=img->ysize) {
    m_restart=true;
  }

  if(m_restart) {
    if(!init(img, framerate)) {
      /* something went wrong! */
      stop();
      pd_error(0, "[GEM:recordQT4L] unable to initialize QT4L");
      return false;
    }
    m_restart=false;
  }

  if(m_curTrack<0) {
    pd_error(0, "[GEM:recordQt4L] detected invalid track %d", m_curTrack);
    return false;
  }

  double timestamp_d=(m_useTimeStamp
                      ?(clock_gettimesince(m_startTime)*TIMEBASE/1000.)
                      :m_curFrame*m_timeTick);

  int64_t timestamp=timestamp_d;

  m_curFrame++;

  switch(m_colormodel) {
  case BC_RGBA8888:
    m_image.convertFrom(img, GEM_RGBA);
    break;
  case BC_RGB888:
    m_image.convertFrom(img, GEM_RGB);
    break;
  case BC_YUV422:
    m_image.convertFrom(img, GEM_YUV);
    break;
  default:
    pd_error(0, "[GEM:recordQT4L] unsupported colormodel...");
    return false;
  }

  row=m_image.ysize;
  row_stride=m_image.xsize*m_image.csize;
  rowpointers=new unsigned char*[row];
  if(!m_image.upsidedown) {
    while(row--) {
      rowpointers[m_image.ysize-row-1]=m_image.data+(row-1)*row_stride;
    }
  } else {
    while(row--) {
      rowpointers[row]=m_image.data+row*row_stride;
    }
  }

  lqt_encode_video(m_qtfile, rowpointers, m_curTrack, timestamp);
  delete[]rowpointers;
  return true;
}



/////////////////////////////////////////////////////////
// get codecs
//
/////////////////////////////////////////////////////////
std::vector<std::string>recordQT4L::getCodecs(void)
{
  std::vector<std::string>result;
  m_codecdescriptions.clear();

  lqt_codec_info_t**codec = lqt_query_registry(0,1,1,0);

  if(codec) {
    int n=0;
    while(NULL!=codec[n]) {
      std::string name=codec[n]->name;
      std::string desc=codec[n]->long_name;
      result.push_back(name);
      m_codecdescriptions[name]=desc;

      n++;
    }

    lqt_destroy_codec_info(codec);
  }

  return result;
}

const std::string recordQT4L::getCodecDescription(const
    std::string&codecname)
{
  return m_codecdescriptions[codecname];
}

/////////////////////////////////////////////////////////
// set codec by name
//
/////////////////////////////////////////////////////////
bool recordQT4L :: setCodec(const std::string&name)
{
  std::string codecname=name;
  // stop();

  m_codec=NULL;

  if(codecname.empty() && m_qtfile) {
    /* LATER figure out automatically which codec to use */
    lqt_file_type_t type = lqt_get_file_type(m_qtfile);
    for(unsigned int i = 0; i < sizeof(qtformats)/sizeof(qtformats[0]); i++) {
      if(type == qtformats[i].type) {
        codecname = qtformats[i].default_video_codec;
      }
    }
    if(codecname.empty()) {
      logpost(0, 3+0, "[GEM:recordQT4L] couldn't find default codec for this format");
      return false;
    }
  }
  lqt_destroy_codec_info(m_codecs);

  m_codecs = lqt_find_video_codec_by_name(codecname.c_str());
  if(m_codecs) {
    m_codec=m_codecs[0];
    m_codecname=codecname;
  }

  bool result=(NULL!=m_codec);
  return result;
}

bool recordQT4L :: enumProperties(gem::Properties&props)
{
  props.clear();
  if(NULL==m_codec) {
    return false;
  }

  props.set("lqtformat", std::string("auto"));

  props.set("name", std::string(""));
  props.set("copyright", std::string(""));
  props.set("info", std::string(""));

  props.set("album", std::string(""));
  props.set("artist", std::string(""));
  props.set("genre", std::string(""));
  props.set("track", std::string(""));
  props.set("comment", std::string(""));
  props.set("author", std::string(""));

  props.set("framerate", 0.f);

  const int paramcount=m_codec->num_encoding_parameters;
  lqt_parameter_info_t*params=m_codec->encoding_parameters;
  for(int i=0; i<paramcount; i++) {
    gem::any typ;
    switch(params[i].type) {
    case(LQT_PARAMETER_INT):
      typ=params[i].val_max.val_int;
      break;
    case(LQT_PARAMETER_FLOAT):
      typ=params[i].val_max.val_float;
      break;
    case(LQT_PARAMETER_STRING):
      typ=params[i].val_default.val_string;
      break;
    default:
      continue;
    }

    props.set(params[i].name, typ);
  }

  return true;
}


#endif
