////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file 
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include <string.h>
#include "Pixes/filmMPEG1.h"

/////////////////////////////////////////////////////////
//
// filmMPEG1
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

filmMPEG1 :: filmMPEG1(int format) : film(format),
				     m_data(NULL), m_length(0)
{
  static bool first_time=true;
  if (first_time) {
#ifdef HAVE_LIBMPEG
    post("pix_film:: libmpeg support");
#endif
    first_time = false;
  }

#ifdef HAVE_LIBMPEG
  m_streamfile=0;
#endif
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
filmMPEG1 :: ~filmMPEG1()
{
  close();
  if(m_data)delete[]m_data;
}

#ifdef HAVE_LIBMPEG
void filmMPEG1 :: close(void)
{
  if (m_streamfile)fclose(m_streamfile);
  m_streamfile=0;
}

/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
bool filmMPEG1 :: open(char *filename, int format)
{
  if (format>0)m_wantedFormat=format;
  if (!(m_streamfile = fopen (filename, "rb")))return false;
  int wantedFormat= (m_wantedFormat)?m_wantedFormat:GL_RGBA;
  switch (wantedFormat){
  case GL_LUMINANCE:
      SetMPEGOption (MPEG_DITHER, GRAY_DITHER);
      break;
  default:
    wantedFormat=GL_RGBA;
  case GL_YCBCR_422_GEM:
  case GL_RGBA:
    SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
  }
  if (OpenMPEG (m_streamfile, &m_streamVid)) { /* let's hope it's MPEG */
    m_curFrame = 0;
    
    // Unfortunately there is no way to get the length of an MPEG-Stream
    m_numFrames = -1;
    
    m_image.image.xsize  = m_streamVid.Width;
    m_image.image.ysize  = m_streamVid.Height;
    if (!(m_image.image.xsize*m_image.image.ysize))goto unsupported;

    m_image.image.setCsizeByFormat(wantedFormat);
    m_image.image.reallocate();

    int length=m_image.image.xsize*m_image.image.ysize;
    length*=((m_image.image.format==GL_LUMINANCE)?1:4)+4;
    if(m_length<length){
      if (m_data)delete[]m_data;
      m_length=length;
      m_data=new unsigned char[m_length];
    }
    m_reachedEnd=false;
    m_newfilm = true;
    return true;
  }
  goto unsupported;
 unsupported:
  post("MPEG1: unsupported!");
  close();
  return false;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
pixBlock* filmMPEG1 :: getFrame(){
  if (m_reachedEnd){
    m_curFrame=-1;
    return 0;
  }
  if (!m_readNext){
    return &m_image;
  }
  m_image.image.upsidedown=true;

  m_readNext = false;
  int length=m_image.image.xsize*m_image.image.ysize;
  length*=((m_image.image.format==GL_LUMINANCE)?1:4)+4;
  if(m_length<length){
    if (m_data)delete[]m_data;
    m_length=length;
    m_data=new unsigned char[m_length];
  }
  if (m_reachedEnd=!GetMPEGFrame ((char*)(m_data))){
    if(m_image.image.format==GL_YCBCR_422_GEM){
      m_image.image.fromRGBA(m_data);
    }  else  m_image.image.data=m_data;
    m_curFrame=-1;
    return &m_image;// was 0; but then we have one non-textured frame in auto-mode
  } else {
    if(m_image.image.format==GL_YCBCR_422_GEM){
      m_image.image.fromRGBA(m_data);
    }  else  m_image.image.data=m_data;
    if(m_newfilm)m_image.newfilm=1;  m_newfilm=false;
    m_image.newimage=1;
    return &m_image;
  }
  return 0;
}

int filmMPEG1 :: changeImage(int imgNum, int trackNum){
  if (m_reachedEnd&&imgNum>0)return FILM_ERROR_FAILURE;

  m_readNext = true;
  if (imgNum==0){
    if (!RewindMPEG(m_streamfile, &m_streamVid))return FILM_ERROR_FAILURE;
    m_curFrame=0;
    m_reachedEnd=false;
  }
  return FILM_ERROR_DONTKNOW;
}
#endif
