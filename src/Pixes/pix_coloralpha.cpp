////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
////////////////////////////////////////////////////////

#include "pix_coloralpha.h"

CPPEXTERN_NEW(pix_coloralpha);

/////////////////////////////////////////////////////////
//
// pix_coloralpha
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_coloralpha :: pix_coloralpha()
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_coloralpha :: ~pix_coloralpha()
{ }

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_coloralpha :: processRGBAImage(imageStruct &image)
{
  // process the image
  int datasize = image.xsize * image.ysize;
  unsigned char *pixel = image.data;

  while(datasize--) {
    /*
    float tempVal = (float)pixel[chRed] + (float)pixel[chGreen] + (float)pixel[chBlue];
    tempVal /= 3.f;
    pixel[chAlpha] = (unsigned char)tempVal;
    pixel += 4;
    */
    int tempVal = pixel[chRed] + pixel[chGreen] + pixel[chBlue];
    tempVal /= 3;
    pixel[chAlpha] = (unsigned char)tempVal;
    pixel += 4;
  }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_coloralpha :: obj_setupCallback(t_class *)
{
  class_addcreator(reinterpret_cast<t_newmethod>(create_pix_coloralpha),
                   gensym("pix_colouralpha"), A_GIMME, A_NULL);
}
