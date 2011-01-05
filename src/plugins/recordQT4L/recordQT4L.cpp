////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file 
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "recordQT4L.h"
using namespace gem;
#include "Base/GemBase.h"
#include "Base/GemMan.h"

#include <stdlib.h>


#ifdef  GEM_USE_RECORDQT4L
REGISTER_RECORDFACTORY("QT4L", recordQT4L);
#endif
/////////////////////////////////////////////////////////
//
// recordQT4L
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

recordQT4L :: recordQT4L(): 
  record()
#if defined  GEM_USE_RECORDQT4L
  ,
  m_qtfile(NULL),
  m_codec(NULL), m_codecs(NULL),
  m_codecname(std::string()), 
  m_qtbuffer(NULL),
  m_colormodel(0),
  m_width(-1), m_height(-1),
  m_restart(true)
{
  lqt_registry_init ();
}
#else
{}
#endif

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
recordQT4L :: ~recordQT4L()
{
  close();
}

#if defined  GEM_USE_RECORDQT4L
void recordQT4L :: close(void)
{
  if(m_qtfile){
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
static struct
{
  const char * name;
  const lqt_file_type_t type;
  const char * extension;
  const char * description;
  const char * default_video_codec;
} qtformats[] =  {
  { "qt",       LQT_FILE_QT,        "mov", "Quicktime (QT7 compatible)",   "yuv2" }, /* ffmpeg_mpg4 */
  { "qtold",    LQT_FILE_QT_OLD,    "mov", "Quicktime (qt4l and old lqt)", "yuv2" }, /* mjpa */
  { "avi",      LQT_FILE_AVI,       "avi", "AVI (< 2G)",                   "yuv2" }, /* ffmpeg_msmpeg4v3 */
  { "avi_odml", LQT_FILE_AVI_ODML,  "avi", "AVI (> 2G)",                   "yuv2" }, /* ffmpeg_msmpeg4v3 */
  { "mp4",      LQT_FILE_MP4,       "mp4", "ISO MPEG-4",                   "yuv2" }, /* ffmpeg_mpg4 */
  { "m4a",      LQT_FILE_M4A,       "m4a", "m4a (iTunes compatible)",      "yuv2"  }, /* ffmpeg_mpg4 */
};
/* guess the file-format by inspecting the extension */
static lqt_file_type_t guess_qtformat(const std::string filename)
{
  const char * extension = strrchr(filename.c_str(), '.');
  unsigned int i=0;

  if(!extension) {
    error("no extension given: encoding will be QuickTime");
    return LQT_FILE_QT;
  }

  extension++;

  for(i = 0; i < sizeof(qtformats)/sizeof(qtformats[0]); i++) {
    if(!strcasecmp(extension, qtformats[i].extension)) {
      post("encoding as %s", qtformats[i].description);
      return qtformats[i].type;
    }
  }
  
  error("unknown extension: encoding will be QuickTime");
  return LQT_FILE_QT; /* should be save for now */
}

bool recordQT4L :: open(const std::string filename)
{
  close();

  lqt_file_type_t type =  guess_qtformat(filename);

  m_qtfile = lqt_open_write(filename.c_str(), type);
  if(m_qtfile==NULL){
    return false;
  }

  m_restart=true;
  return (true);
}

/////////////////////////////////////////////////////////
// initialize the encoder
//
/////////////////////////////////////////////////////////
bool recordQT4L :: init(const imageStruct*img, const int framedur)
{
  int rowspan=0, rowspan_uv=0;
  lqt_codec_info_t*codec=NULL;

  if(!m_qtfile || !img || framedur < 0)
    return false;

  post("%s:%d: %p", __FUNCTION__, __LINE__, m_codec); 
  /* do we have a codec specified? */
  if(NULL==m_codec) {
    setCodec(m_codecname);
  }

  if(NULL==m_codec) {
    error("couldn't initialize codec");
    return false;
  }
  
  /* fps = time_scale / frame_duration */
  lqt_set_video(m_qtfile,
                1, 
                img->xsize, 
                img->ysize,
                1,
                framedur,
                m_codec);

  /* set the colormodel */
  m_colormodel=BC_RGB888; /* LATER do this more dynamically */

  /* make sure to allocate enough buffer; it sometimes crashes when i allocate the "right" size, 
     so we just grab a multiple of what we actually want... 
  */
  /* but isn't this a memleak? it sure crashes if i try to lqt_rows_free() the qtbuffer */
  m_qtbuffer = lqt_rows_alloc(2*img->xsize, 2*img->ysize, m_colormodel, &rowspan, &rowspan_uv);

  quicktime_set_cmodel(m_qtfile, m_colormodel);

  m_width =img->xsize;
  m_height=img->ysize;

  return true;
}

/////////////////////////////////////////////////////////
// do the actual encoding and writing to file
//
/////////////////////////////////////////////////////////
bool recordQT4L :: putFrame(imageStruct*img)
{
  post("%s:%d:: %p %p", __FUNCTION__, __LINE__, m_qtfile, img);
  if(!m_qtfile || !img){
    return false;
  }
  unsigned char**rowpointers;
  int row, row_stride;
  float framerate = GemMan::getFramerate();

  if(m_width!=img->xsize || m_height!=img->ysize)m_restart=true;

  if(m_restart){
    if(!init(img, static_cast<int>(framerate))) {
      /* something went wrong! */
      close();
      error("unable to initialize QT4L");
      return false;
    }
    m_restart=false;
  }

  switch(m_colormodel){
  case BC_RGBA8888:
    m_image.convertFrom(img, GL_RGBA);
    break;
  case BC_RGB888:
    m_image.convertFrom(img, GL_RGB);
    break;
  case BC_YUV422:
    m_image.convertFrom(img, GL_YUV422_GEM);
    break;
  default:
    error("record: unsupported colormodel...");
    return false;
  }

  row=m_image.ysize;
  row_stride=m_image.xsize*m_image.csize;
  rowpointers=new unsigned char*[row];
  if(!m_image.upsidedown){
    while(row--){
      rowpointers[m_image.ysize-row-1]=m_image.data+(row-1)*row_stride;
    }
  } else {
    while(row--){
      rowpointers[row]=m_image.data+row*row_stride;
    }
  }

  lqt_encode_video(m_qtfile, rowpointers, 0, static_cast<int>(framerate));
  delete[]rowpointers;
  return true;
}



/////////////////////////////////////////////////////////
// get codecs
//
/////////////////////////////////////////////////////////
std::vector<std::string>recordQT4L::getCodecs() {
  std::vector<std::string>result;
  m_codecdescriptions.clear();

  lqt_codec_info_t**codec = lqt_query_registry(0,1,1,0);

  if(codec) {
    int n=0;
    while(NULL!=codec[n]){
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

/////////////////////////////////////////////////////////
// set codec by name
//
/////////////////////////////////////////////////////////
bool recordQT4L :: setCodec(const std::string name)
{
  std::string codecname=name;
  stop();

  m_codec=NULL;

  if(codecname.empty() && m_qtfile) {
    /* LATER figure out automatically which codec to use */ 
    lqt_file_type_t type = lqt_get_file_type(m_qtfile);
    unsigned int i=0;
    for(i = 0; i < sizeof(qtformats)/sizeof(qtformats[0]); i++) {
      if(type == qtformats[i].type){
	codecname = qtformats[i].default_video_codec;
      }
    }
    if(codecname.empty()) {
      error("couldn't find default codec for this format");
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
  post("setCodec('%s')=%p returns %d", name.c_str(), m_codec, result);
  return result;
}

bool recordQT4L :: enumProperties(gem::Properties&props) 
{
  props.clear();
  post("enumprops %p", m_codec);
  if(NULL==m_codec)
    return false;

  const int paramcount=m_codec->num_encoding_parameters;
  post("codec has %d encoding params", paramcount);
  lqt_parameter_info_t*params=m_codec->encoding_parameters;
  int i=0;
  for(i=0; i<paramcount; i++) {
    gem::any typ;
    switch(params[i].type) {
    case(LQT_PARAMETER_INT):
    case(LQT_PARAMETER_FLOAT):
      typ=0;
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
