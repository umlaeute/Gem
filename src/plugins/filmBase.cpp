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

#include "filmBase.h"
#include <stdlib.h>

/////////////////////////////////////////////////////////
//
// film
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

namespace gem { namespace plugins {

class filmBase :: PIMPL {
public:
  bool m_thread;
  PIMPL(bool thread) :
    m_thread(thread)
  {  };
};

  filmBase :: filmBase(bool threadable) : 
  m_pimpl(new PIMPL(threadable)),
#ifdef __APPLE__
  m_wantedFormat(GL_YUV422_GEM), 
#else
  m_wantedFormat(GL_RGBA), 
#endif
  m_numFrames(0), m_numTracks(0),
  m_curFrame(0), m_curTrack(0),
  m_readNext(false),
  m_auto(0.f),
  m_fps(-1.0),
  m_newfilm(false)
{}

filmBase :: filmBase(void) : 
  m_pimpl(new PIMPL(true)),
#ifdef __APPLE__
  m_wantedFormat(GL_YUV422_GEM), 
#else
  m_wantedFormat(GL_RGBA), 
#endif
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
filmBase :: ~filmBase()
{
  // you must call close in the dtor! of the derived class
  delete m_pimpl; m_pimpl=NULL;
}

bool filmBase :: isThreadable(void) {
  return m_pimpl->m_thread;
}

void filmBase :: close(void)
{}

/////////////////////////////////////////////////////////
// do we have a film loaded ?
//
/////////////////////////////////////////////////////////
bool filmBase :: haveFilm()
{
  return false;
}
#if 0
/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
pixBlock* filmBase :: getFrame(){
  if(m_newfilm)m_image.newfilm=1;  m_newfilm=false;
  m_image.newimage=1;
  unsigned char *dummy=m_image.image.data;
  int i = m_image.image.xsize*m_image.image.ysize*m_image.image.csize;
  while(i--)*dummy++=rand()%256;
  return &m_image;
}
#endif
///////////////////////////////
// get the frames-per-second
double filmBase :: getFPS() {
  // we don't know, so we return "-1"
  return m_fps;
}

void filmBase :: setAuto(double incr) {
  m_auto = incr;
}

filmBase::errCode filmBase :: changeImage(int imgNum, int trackNum){
  return DONTKNOW;
}

};}; // namespace

