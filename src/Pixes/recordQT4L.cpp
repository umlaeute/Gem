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

#include "Pixes/recordQT4L.h"
#include "Base/GemBase.h"

#include <stdlib.h>

/////////////////////////////////////////////////////////
//
// recordQT4L
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

recordQT4L :: recordQT4L(int x, int y, int width, int height): record(x,y,width,height)
#if defined HAVE_LIBQUICKTIME && defined HAVE_LQT_ADD_VIDEO_TRACK
							     , m_qtfile(NULL),
							       m_codec(NULL), m_codecs(NULL),
							       m_track(-1), m_colormodel(0)
{
  //  strcpy(m_compressor, QUICKTIME_DV);
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
#if defined HAVE_LIBQUICKTIME && defined HAVE_LQT_ADD_VIDEO_TRACK
void recordQT4L :: close(void)
{
  if(m_qtfile){
    quicktime_close(m_qtfile);
    m_qtfile=NULL;
  }
}
/////////////////////////////////////////////////////////
// open a file !
//
/////////////////////////////////////////////////////////
bool recordQT4L :: open(char *filename)
{
  close();
  m_qtfile = quicktime_open(filename, 0, 1);
  if(m_qtfile==NULL){
    return false;
  }
  m_currentFrame=0;
  m_track=-1;
  return (true);
}

/////////////////////////////////////////////////////////
// get number of codecs
//
/////////////////////////////////////////////////////////
int recordQT4L :: getNumCodecs()
{
  lqt_codec_info_t**codecs = lqt_query_registry(0,1,1,0);
  int n=0;
  while(NULL!=codecs[n])n++;
  lqt_destroy_codec_info(codecs);
  return (n);
}
char*recordQT4L :: getCodecName(int i)
{
  lqt_codec_info_t**codec = lqt_query_registry(0,1,1,0);
  if(codec&&codec[i]){
    char*name=gensym(codec[i]->name)->s_name;
    lqt_destroy_codec_info(codec);
    return name;
  }
  return NULL;
}
char*recordQT4L :: getCodecDescription(int i)
{
  lqt_codec_info_t**codec = lqt_query_registry(0,1,1,0);
  if(codec&&codec[i]){
    char*name=gensym(codec[i]->long_name)->s_name;
    lqt_destroy_codec_info(codec);
    return name;
  }
  return NULL;
}
/////////////////////////////////////////////////////////
// set codec by number
//
/////////////////////////////////////////////////////////
bool recordQT4L :: setCodec(int num)
{
  lqt_codec_info_t**codec = lqt_query_registry(0,1,1,0);
  int n=0;
  if(!codec||!codec[0])return false;

  while(codec[n])n++;
  if(num<0||num>=n){
    lqt_destroy_codec_info(codec);
    return false;
  }

  if(m_codecs)lqt_destroy_codec_info(m_codecs);
  
  m_codecs=codec;
  m_codec=codec[num];
  m_width=m_height=-1;
  return true;
}
/////////////////////////////////////////////////////////
// set codec by name
//
/////////////////////////////////////////////////////////
bool recordQT4L :: setCodec(char*name)
{
  lqt_codec_info_t**codec = (lqt_codec_info_t**)lqt_find_video_codec_by_name(name);
  if(!codec || !codec[0]){
    return false;
  }

  if(m_codecs)lqt_destroy_codec_info(m_codecs);
  
  m_codecs=codec;
  m_codec=codec[0];
  m_width=m_height=-1;
  return true;

}

int recordQT4L :: putFrame(imageStruct*img)
{
  if(!m_qtfile || !img)return (-1);
  unsigned char**rowpointers;
  int row, row_stride;
  int err;
  bool restart=false;

  if(m_width!=img->xsize || m_height!=img->ysize)restart=true;

  if(!m_codec){
    if (!setCodec((char*)"raw"))return(-1);
    restart=true;
  }


  if(restart){
    err=lqt_add_video_track(m_qtfile, img->xsize, img->ysize, 1000, 20000, m_codec);
    if(err!=0)return -1;
    m_width =img->xsize;
    m_height=img->ysize;
    m_track++;
    /* JMZ: raw seems to be happy only with 32-bit RGB values
     * whereas most other codecs cannot really handle this
     * in theory lqt>=0.9.8 should be able to query which colormodels are supported
     * but unfortunately 0.9.8 was released in mid january 2006 (now it is february)
     * on debian i still have 0.9.7
     * the real bad thing is, that i cannot handle this via preprocessor
     */
#if 0
    int supported_cm[]={BC_RGBA8888, BC_YUV422, BC_RGB888, LQT_COLORMODEL_NONE};
    int cm=lqt_get_cmodel(m_qtfile, m_track);
    m_colormodel=lqt_get_best_cmodel(m_qtfile, m_track, supported_cm);
#else
    if(gensym("raw")==gensym(m_codec->name))
      m_colormodel=BC_RGBA8888;
    else
      m_colormodel=BC_RGB888;
#endif
    lqt_set_cmodel(m_qtfile, m_track, m_colormodel);
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
    return (-1);
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

  //  lqt_encode_video(m_qtfile, rowpointers, m_track, 1000);
  quicktime_encode_video(m_qtfile, rowpointers, m_track);

  //  quicktime_encode_video(m_qtfile, rowpointers, 0);
  m_currentFrame++;
  return m_currentFrame;
}
#endif
