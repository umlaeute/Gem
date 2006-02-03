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
#ifdef HAVE_LIBQUICKTIME
							     , m_qtfile(NULL)
#endif
{
  strcpy(m_compressor, QUICKTIME_DV);
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
recordQT4L :: ~recordQT4L()
{
  close();
}
#ifdef HAVE_LIBQUICKTIME
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
  return (true);
}

/////////////////////////////////////////////////////////
// get number of codecs
//
/////////////////////////////////////////////////////////
int recordQT4L :: getNumCodecs()
{
  return(8);
}
char*recordQT4L :: getCodecName(int i)
{
  switch(i){
  default:return NULL;
  case 0: return (gensym("divx")->s_name);
  case 1: return (gensym("dv")->s_name);
  case 2: return (gensym("raw")->s_name);
  case 3: return (gensym("jpeg")->s_name);
  case 4: return (gensym("png")->s_name);
  case 5: return (gensym("mjpa")->s_name);
  case 6: return (gensym("yuv2")->s_name);
  case 7: return (gensym("yuv4")->s_name);
  }
  return NULL;
}
char*recordQT4L :: getCodecDescription(int i)
{
  switch(i){
  default:return NULL;
  case 0: return (gensym("divx")->s_name);
  case 1: return (gensym("dv")->s_name);
  case 2: return (gensym("raw")->s_name);
  case 3: return (gensym("jpeg")->s_name);
  case 4: return (gensym("png")->s_name);
  case 5: return (gensym("mjpa")->s_name);
  case 6: return (gensym("yuv2")->s_name);
  case 7: return (gensym("yuv4")->s_name);
  }
  return NULL;
}
/////////////////////////////////////////////////////////
// set codec by number
//
/////////////////////////////////////////////////////////
bool recordQT4L :: setCodec(int num)
{
  post("qt4l: set codec %d", num);
  switch(num){
  default:return false;
  case 0: strcpy(m_compressor, QUICKTIME_DIVX); break;
  case 1: strcpy(m_compressor, QUICKTIME_DV); break;
  case 2: strcpy(m_compressor, QUICKTIME_RAW); break;
  case 3: strcpy(m_compressor, QUICKTIME_JPEG); break;
  case 4: strcpy(m_compressor, QUICKTIME_PNG); break;
  case 5: strcpy(m_compressor, QUICKTIME_MJPA); break;
  case 6: strcpy(m_compressor, QUICKTIME_YUV2); break;
  case 7: strcpy(m_compressor, QUICKTIME_YUV4); break;
  }
  return true;
}
/////////////////////////////////////////////////////////
// set codec by name
//
/////////////////////////////////////////////////////////
bool recordQT4L :: setCodec(char*name)
{
  return false;
}

int recordQT4L :: putFrame(imageStruct*img)
{
  if(!m_qtfile)return (-1);
  if(img==NULL)return (-1);
  unsigned char**rowpointers;
  int row, row_stride;
  int err;
  m_fps=25.f;

  m_image.convertFrom(img, GL_RGBA);

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

  quicktime_set_cmodel(m_qtfile, BC_RGBA8888 );
  quicktime_set_copyright(m_qtfile, "GnuGPL");
  quicktime_set_name(m_qtfile, "Graphics Environment for Multimedia");
  quicktime_set_info  (m_qtfile, "created with pd/Gem");
  quicktime_set_framerate(m_qtfile, m_fps );
  if( ( err = quicktime_set_video(m_qtfile, 1, m_image.xsize, m_image.ysize, m_fps, m_compressor) ) != 0) {
    return (-1);
  }
  quicktime_encode_video(m_qtfile, rowpointers, 0);
  m_currentFrame++;
  return m_currentFrame;
}
#endif
