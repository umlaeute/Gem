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
  m_data = NULL;
  m_size = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_grey :: ~pix_grey()
{
  if (m_data) delete [] m_data;
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_grey :: processImage(imageStruct &image)
{
  if (image.format==GL_LUMINANCE)return;
  unsigned char *old_pix, *new_pix;
  int count = image.xsize*image.ysize;
  int format = image.format;
  int csize = image.csize;

  if (image.xsize*image.ysize != m_size){
    delete [] m_data;
    m_size = image.xsize*image.ysize;
    m_data = new unsigned char[m_size];
  }

  old_pix = image.data;
  new_pix = m_data;

  switch (format){
  case GL_LUMINANCE:
    break;
  case GL_RGBA:
  case GL_RGB:  
    while(count--){
      *new_pix++ =(unsigned char)(old_pix[chRed]*0.3086f + 
				  old_pix[chGreen]*0.6094f + 
				  old_pix[chBlue]*0.0820f);
      old_pix+=csize;
    }
    break;
  case GL_BGR_EXT:
  case GL_BGRA_EXT:
    while(count--){
      *new_pix++ =(unsigned char)(old_pix[chRed]*0.3086f + 
				  old_pix[chGreen]*0.6094f + 
				  old_pix[chBlue]*0.0820f);
      old_pix+=csize;
    }
    break;
  case GL_YCBCR_422_GEM: // YUV just take luminance
    old_pix++;
    while (count--){
      *new_pix++ = *(old_pix+=2);
    }
    break;
  default:
    post("no method for this format !!!");
    post("if you know how to convert this format (%X) to GREY,\n"
	 "please contact the authors of this software", format);
    return;
  }

  image.data   = m_data;
  image.notowned = 1;
  image.format = GL_LUMINANCE;
  image.csize  = 1;
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_grey :: obj_setupCallback(t_class *classPtr)
{
}
