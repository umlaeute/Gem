////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
// this is based on EffecTV by Fukuchi Kentarou
// * Copyright (C) 2001 FUKUCHI Kentarou
//
/////////////////////////////////////////////////////////

#include "pix_grey.h"

CPPEXTERN_NEW(pix_grey)

/////////////////////////////////////////////////////////
//
// pix_grey
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_grey :: pix_grey()
{
  m_image.xsize=64;
  m_image.ysize=64;
  m_image.setCsizeByFormat(GL_LUMINANCE);
  m_image.reallocate();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_grey :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_grey, 
		   gensym("pix_gray"), A_NULL);
}
