////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyleft  (l) 2001 IOhannes m zmölnig
//    Copyleft (l) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_fx.h"

CPPEXTERN_NEW(pix_fx)

/////////////////////////////////////////////////////////
//
// pix_fx
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_fx :: pix_fx()
{
  m_active = true;

  oldImage = 0;
  old_data = 0;
  old_xsize = 0;
  old_ysize = 0;
  old_csize = 0;
  old_format = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_fx :: ~pix_fx()
{ }

/////////////////////////////////////////////////////////
// ProcessImage: insert the FX into the chain
//
/////////////////////////////////////////////////////////
void pix_fx :: processImage(imageStruct &image)
{
  oldImage  = &image;
  old_data  = image.data;
  old_xsize = image.xsize;
  old_ysize = image.ysize;
  old_csize = image.csize;
  old_format= image.format;

  if (m_active)processFX(image);
}
/////////////////////////////////////////////////////////
// ProcessImage: insert the FX into the chain
//
/////////////////////////////////////////////////////////
void pix_fx :: processGrayImage(imageStruct &image)
{
  oldImage  = &image;
  old_data  = image.data;
  old_xsize = image.xsize;
  old_ysize = image.ysize;
  old_csize = image.csize;
  old_format= image.format;

  if (m_active)processFX(image);
}
/////////////////////////////////////////////////////////
// ProcessImage: insert the FX into the chain
//
/////////////////////////////////////////////////////////
void pix_fx :: processYUVImage(imageStruct &image)
{
  post("pix_fx: YUVImage via pix_fx not supported yet");
/*
  oldImage  = &image;
  old_data  = image.data;
  old_xsize = image.xsize;
  old_ysize = image.ysize;
  old_csize = image.csize;
  old_format= image.format;

  if (m_active)processYUVfx(image);
*/
}
//////////
// get the original state back
void pix_fx :: postrender(GemState *state){
  if (!oldImage)return;

  oldImage->data  = old_data;
  oldImage->xsize = old_xsize;
  oldImage->ysize = old_ysize;
  oldImage->csize = old_csize;
  oldImage->format= old_format;
}

void pix_fx :: activate(int state){
  m_active=(!state);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_fx :: obj_setupCallback(t_class *classPtr)
{
  class_addfloat(classPtr, (t_method)&pix_fx::stateMessCallback);
}

void pix_fx :: stateMessCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->activate(state);
}
