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

#include "pix_rgba.h"

CPPEXTERN_NEW(pix_rgba)

/////////////////////////////////////////////////////////
//
// pix_rgba
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_rgba :: pix_rgba()
{
  m_image.xsize=64;
  m_image.ysize=64;
#ifdef __APPLE__
  m_image.setCsizeByFormat(GL_BGRA_EXT);
#else
  m_image.setCsizeByFormat(GL_RGBA);
#endif
  m_image.reallocate();
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_rgba :: ~pix_rgba()
{}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_rgba :: processImage(imageStruct &image)
{
  if (image.format==m_image.format)return;
  m_image.xsize=image.xsize;
  m_image.ysize=image.ysize;
 
  switch (image.format){
  case GL_RGBA: 
    m_image.fromRGBA(image.data);
    break;
  case GL_RGB:  
    m_image.fromRGB(image.data);
    break;
  case GL_BGR_EXT:
    m_image.fromBGR(image.data);
    break;
  case GL_BGRA_EXT: /* "RGBA" on apple */
    m_image.fromBGRA(image.data);
    break;
  case GL_LUMINANCE:
    m_image.fromGray(image.data);
    break;
  case GL_YCBCR_422_GEM: // YUV
    m_image.fromUYVY(image.data);
    break;
  default:
    error("[%s] has no method for this format !!!", m_objectname);
    error("if you know how to convert this format (%X),\n"
	 "please contact the authors of this software", image.format);
    return;
  }

  image.data   = m_image.data;
  image.notowned = 0;
  image.setCsizeByFormat(m_image.format);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_rgba :: obj_setupCallback(t_class *classPtr)
{
}
