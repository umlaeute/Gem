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

filmMPEG1 :: filmMPEG1(int format) : film(format) {
  static bool first_time=true;
  if (first_time) {
#ifdef HAVE_LIBMPEG
    post("pix_film:: libmpeg support");
#endif
    first_time = false;
  }

  m_streamfile=0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
filmMPEG1 :: ~filmMPEG1()
{
  close();
}

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
#ifdef HAVE_LIBMPEG
  int wantedFormat= (m_wantedFormat)?m_wantedFormat:GL_RGBA;
  switch (wantedFormat){
  case GL_LUMINANCE:
      SetMPEGOption (MPEG_DITHER, GRAY_DITHER);
      break;
  case GL_YCBCR_422_GEM:
    //      SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
    //break;
  default:
    wantedFormat=GL_RGBA;
  case GL_RGBA:
    SetMPEGOption (MPEG_DITHER, FULL_COLOR_DITHER);
  }
  if (OpenMPEG (m_streamfile, &m_streamVid)) { /* let's hope it's MPEG */
    m_curFrame = 0;
    
    // Unfortunately there is no way to get the length of an MPEG-Stream
    m_numFrames = -1;
    
    m_image.image.xsize  = m_streamVid.Width;
    m_image.image.ysize  = m_streamVid.Height;
    switch (wantedFormat){
    case GL_LUMINANCE:
      m_image.image.csize  = 1;
      m_image.image.format = GL_LUMINANCE;
      break;
#if 0
    case GL_YCBCR_422_GEM:
      m_image.image.csize  = 2;
      m_image.image.format = GL_YCBCR_422_GEM;
      break;
#endif
    case GL_RGBA:
    default:
      m_image.image.csize  = 4;
      m_image.image.format = GL_RGBA;
    }
    if (!(m_image.image.xsize*m_image.image.ysize*m_image.image.csize))goto unsupported;

    m_image.image.reallocate();
    m_reachedEnd=false;
    post("MPEG1 opened");
    return true;
  }
#endif
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
  m_readNext = false;

#ifdef HAVE_LIBMPEG
  if (m_reachedEnd=!GetMPEGFrame ((char*)(m_image.image.data))){
    m_curFrame=-1;
    return &m_image;// was 0; but then we have one non-textured frame in auto-mode
  } else {
    m_image.newimage=1;
    return &m_image;
  }
#endif
  return 0;
}

int filmMPEG1 :: changeImage(int imgNum, int trackNum){
  if (m_reachedEnd&&imgNum>0)return FILM_ERROR_FAILURE;

  m_readNext = true;
  if (imgNum==0){
#ifdef HAVE_LIBMPEG
    if (!RewindMPEG(m_streamfile, &m_streamVid))return FILM_ERROR_FAILURE;
#endif
    m_curFrame=0;
    m_reachedEnd=false;
  }
  return FILM_ERROR_DONTKNOW;
}
