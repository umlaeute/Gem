////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 2002, Chris Clepper and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
////////////////////////////////////////////////////////

#include "pix_emboss.h"

CPPEXTERN_NEW(pix_emboss);

/////////////////////////////////////////////////////////
//
// pix_emboss
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_emboss :: pix_emboss()
{
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_emboss :: ~pix_emboss()
{}

/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void pix_emboss :: processYUVImage(imageStruct &image)
{
  int width = image.xsize-1;
  int height = image.ysize-1;
  int w = image.xsize*2;
  int src = 1;
  for (int r=0; r < height; r++) {
    for (int c=0; c < width; c++) {
      image.data[src] = ( image.data[src-w-2 ] -
                          image.data[src-w   ] -
                          image.data[src-w+2 ] +
                          image.data[src - 2 ] -
                          image.data[src     ] -
                          image.data[src + 2 ] -
                          image.data[src+w-2 ] -
                          image.data[src+w   ] -
                          image.data[src+w+2 ] )/4;
      src+=2;
    }
  }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_emboss :: obj_setupCallback(t_class *classPtr)
{

}
