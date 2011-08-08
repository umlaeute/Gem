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

#include <string.h>
#include "filmTEST.h"
#include "plugins/PluginFactory.h"
#include "Gem/RTE.h"

using namespace gem::plugins;

REGISTER_FILMFACTORY("test", filmTEST);

/////////////////////////////////////////////////////////
//
// filmTEST
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

filmTEST :: filmTEST(void) : filmBase(),
			     m_data(NULL), m_length(0)
{
  m_image.image.setCsizeByFormat(GL_RGBA);
  m_image.image.xsize=320;
  m_image.image.ysize=240;
  m_image.image.reallocate();
}

/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
bool filmTEST :: open(const std::string filename, int format)
{
  m_numTracks=1,

  m_curFrame=0;
  m_numFrames=100;
  m_fps=20;
  m_newfilm=true;
  
  changeImage(0,0);

  return true;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
pixBlock* filmTEST :: getFrame(){
  return &m_image;
}

film::errCode filmTEST :: changeImage(int imgNum, int trackNum){
  unsigned char*data=m_image.image.data;
  unsigned int size=m_image.image.xsize*m_image.image.ysize*m_image.image.csize;

  unsigned char value=(unsigned char)imgNum;

  while(size-->0) {
    *data++=value;
    value++;
  }

  m_image.newimage=true;

  return film::SUCCESS;
}
