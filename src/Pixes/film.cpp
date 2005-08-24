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

#include "Pixes/film.h"
#include <stdlib.h>

/////////////////////////////////////////////////////////
//
// film
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

film :: film(int format) : 
  m_wantedFormat(format), 
  m_curFrame(0), m_curTrack(0),
  m_fps(-1.0)
{}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
film :: ~film()
{
  close();
}

void film :: close(void)
{}

/////////////////////////////////////////////////////////
// open a file !
//
/////////////////////////////////////////////////////////
bool film :: open(char *filename, int format)
{
  return false;
  if (format!=0)m_wantedFormat=format;
  int wantedFormat = (m_wantedFormat)?m_wantedFormat:GL_RGBA;
  m_numFrames=0;
  m_image.image.xsize=64;
  m_image.image.ysize=64;
  m_image.image.type=GL_UNSIGNED_BYTE;

  switch (wantedFormat){
  case GL_RGB:
#ifdef GL_BGR
  case GL_BGR:
#endif
    m_image.image.csize=3;
    m_image.image.format=wantedFormat;
    break;
  case GL_LUMINANCE:
    m_image.image.csize=1;
    m_image.image.format=wantedFormat;
    break;
  default:
    wantedFormat=GL_RGBA;
  case GL_RGBA:    
#ifdef GL_BGRA
  case GL_BGRA:
#endif
    m_image.image.csize=4;
    m_image.image.format=wantedFormat;
  }
  m_image.image.allocate();

  m_newfilm = true;

  return true;
}

/////////////////////////////////////////////////////////
// do we have a film loaded ?
//
/////////////////////////////////////////////////////////
bool film :: haveFilm()
{
  return false;
}
/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
pixBlock* film :: getFrame(){
  if(m_newfilm)m_image.newfilm=1;  m_newfilm=false;
  m_image.newimage=1;
  unsigned char *dummy=m_image.image.data;
  int i = m_image.image.xsize*m_image.image.ysize*m_image.image.csize;
  while(i--)*dummy++=rand()%256;
  return &m_image;
}

///////////////////////////////
// get the frames-per-second
double film :: getFPS() {
  // we don't know, so we return "-1"
  return m_fps;
}

int film :: changeImage(int imgNum, int trackNum){
  return FILM_ERROR_DONTKNOW;
}
