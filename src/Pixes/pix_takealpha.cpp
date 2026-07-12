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

#include "pix_takealpha.h"

CPPEXTERN_NEW(pix_takealpha);

/////////////////////////////////////////////////////////
//
// pix_takealpha
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_takealpha :: pix_takealpha()
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_takealpha :: ~pix_takealpha()
{ }

/////////////////////////////////////////////////////////
// processDualImage
//
/////////////////////////////////////////////////////////
void pix_takealpha :: processRGBA_RGBA(imageStruct &image,
                                       imageStruct &right)
{
  int datasize = image.xsize * image.ysize;
  unsigned char *leftPix = image.data;
  unsigned char *rightPix = right.data;

  while(datasize--)    {
    leftPix[chAlpha] =rightPix[chAlpha];
    leftPix += 4;
    rightPix += 4;
  }
}

/////////////////////////////////////////////////////////
// processRightGray
//
/////////////////////////////////////////////////////////
void pix_takealpha :: processRGBA_Gray(imageStruct &image,
                                       imageStruct &right)
{
  int datasize = image.xsize * image.ysize;
  unsigned char *leftPix = image.data;
  unsigned char *rightPix = right.data;

  while(datasize--) {
    leftPix[chAlpha] = rightPix[chGray];
    leftPix += 4;
    rightPix+= 1;
  }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_takealpha :: obj_setupCallback(t_class *)
{ }
