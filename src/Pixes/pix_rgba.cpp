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
  m_data = NULL;
  m_size = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_rgba :: ~pix_rgba()
{
  if (m_data) delete [] m_data;
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_rgba :: processImage(imageStruct &image)
{
  if (image.format==GL_RGBA)return;
  unsigned char *old_pix, *new_pix;
  int count = image.xsize*image.ysize;
  int format = image.format;

  if (image.xsize*image.ysize != m_size){
    delete [] m_data;
    m_size = image.xsize*image.ysize;
    m_data = new unsigned char[m_size*4];
  }

  old_pix = image.data;
  new_pix = m_data;

  switch (format){
  case GL_RGBA: /* nothing to do */
    break;
  case GL_RGB:  

    while(count--){
      new_pix[chRed]  = old_pix[0];
      new_pix[chGreen]= old_pix[1];
      new_pix[chBlue] = old_pix[2];
      new_pix[chAlpha]= 255;
      new_pix+=4;
      old_pix+=3;
    }
    break;
  case GL_BGR_EXT:
    while(count--){
      new_pix[chRed]  =old_pix[2];
      new_pix[chGreen]=old_pix[1];
      new_pix[chBlue] =old_pix[0];
      new_pix[chAlpha]=255;
      old_pix+=3;
      new_pix+=4;
    }
    break;
  case GL_BGRA_EXT:
    while(count--){
      new_pix[chRed]  =old_pix[2];
      new_pix[chGreen]=old_pix[1];
      new_pix[chBlue] =old_pix[0];
      new_pix[chAlpha]=old_pix[3];
      old_pix+=4;
      new_pix+=4;
    }
    break;
    
  case GL_LUMINANCE:
    while(count--){
      new_pix[chRed]  =old_pix[0];
      new_pix[chGreen]=old_pix[0];
      new_pix[chBlue] =old_pix[0];
      new_pix[chAlpha]=255;
      old_pix++;
      new_pix+=4;
    }
    break;
  default:
    post("no method for this format !!!");
    post("if you know how to convert this format (%X) to RGBA,\n"
	 "please contact the authors of this software", format);
    return;
  }


  image.data   = m_data;
  image.notowned = 1;
  image.format = GL_RGBA;
  image.csize  = 4;

}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_rgba :: obj_setupCallback(t_class *classPtr)
{
}
