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

#include "film.h"
#include <stdlib.h>

/////////////////////////////////////////////////////////
//
// film
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

namespace gem {

film :: film(GLenum format) : 
  m_wantedFormat(format), 
  m_numFrames(0), m_numTracks(0),
  m_curFrame(0), m_curTrack(0),
  m_readNext(false),
  m_auto(0.f),
  m_fps(-1.0),
  m_newfilm(false)
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

void film :: setAuto(t_float incr) {
  m_auto = incr;
}

int film :: changeImage(int imgNum, int trackNum){
  return FILM_ERROR_DONTKNOW;
}

}

INIT_FILMFACTORY();


