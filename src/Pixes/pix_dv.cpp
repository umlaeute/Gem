////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_dv.h"
#ifndef DV_PAL
#define DV_PAL 1
#define DV_NTSC 0
#endif

CPPEXTERN_NEW(pix_dv)

  /////////////////////////////////////////////////////////
//
// pix_dv
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_dv :: pix_dv(void)
{
  static int already_in_use=0;
  if (already_in_use){
    error("GEM: only one 'pix_dv' can be used at a time...");
    m_haveVideo=0;
    return;
  }

  already_in_use=1;

  m_norm = DV_PAL;
  m_pixBlock.image.data  = NULL;
  m_pixBlock.image.xsize = m_pixBlock.image.ysize=0;
  m_pixBlock.image.csize = 3;
  m_pixBlock.image.format= GL_RGB; /* we won't get RGBA, so we do not even try...*/

  m_haveVideo=1;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_dv :: ~pix_dv()
{
  cleanPixBlock();
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_dv :: startRendering()
{
  startTransfer();
  m_pixBlock.newimage = 1;
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_dv :: stopRendering()
{
  stopTransfer();
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_dv :: postrender(GemState *state)
{
  m_pixBlock.newimage = 0;
  state->image = NULL;
}

/////////////////////////////////////////////////////////
// cleanPixBlock -- free the pixel buffer memory
//
/////////////////////////////////////////////////////////
void pix_dv :: cleanPixBlock()
{
  m_pixBlock.image.clear();
  m_pixBlock.image.data = NULL;
  m_pixBlock.image.xsize=m_pixBlock.image.ysize=0;
}
/////////////////////////////////////////////////////////
// cleanPixBlock -- free the pixel buffer memory
//
/////////////////////////////////////////////////////////
void pix_dv :: makePixBlock()
{
  int num_pix = 720*(m_norm==DV_PAL)?576:480;

  if (num_pix != m_pixBlock.image.xsize*m_pixBlock.image.ysize){
    cleanPixBlock();
    m_pixBlock.image.xsize=720;
    m_pixBlock.image.ysize=(m_norm==DV_PAL)?576:480;
    m_pixBlock.image.csize=3;
    m_pixBlock.image.format = GL_RGB;
    m_pixBlock.image.allocate(num_pix*m_pixBlock.image.csize);
  }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_dv :: obj_setupCallback(t_class *classPtr)
{
}
