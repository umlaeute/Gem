////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_diff.h"
#include <stdlib.h>

CPPEXTERN_NEW(pix_diff)

/////////////////////////////////////////////////////////
//
// pix_diff
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_diff :: pix_diff()
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_diff :: ~pix_diff()
{ }

/////////////////////////////////////////////////////////
// processDualImage
//
/////////////////////////////////////////////////////////
void pix_diff :: processRGBA_RGBA(imageStruct &image, imageStruct &right)
{
    int datasize = image.xsize * image.ysize;
    unsigned char *leftPix = image.data;
    unsigned char *rightPix = right.data;

    while(datasize--)    {
      leftPix[chRed] =
	abs(leftPix[chRed] - (int)rightPix[chRed]);
      leftPix[chGreen] =
	abs(leftPix[chGreen] - (int)rightPix[chGreen]);
      leftPix[chBlue] =
	abs((int)leftPix[chBlue] - (int)rightPix[chBlue]);
      leftPix += 4;
      rightPix += 4;
    }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_diff :: obj_setupCallback(t_class *)
{ }
