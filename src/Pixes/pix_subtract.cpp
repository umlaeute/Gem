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
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_subtract.h"

CPPEXTERN_NEW(pix_subtract)

/////////////////////////////////////////////////////////
//
// pix_subtract
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_subtract :: pix_subtract()
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_subtract :: ~pix_subtract()
{ }

/////////////////////////////////////////////////////////
// processDualImage
//
/////////////////////////////////////////////////////////
void pix_subtract :: processRGBA_RGBA(imageStruct &image, imageStruct &right)
{
#ifndef MMX
    int datasize = image.xsize * image.ysize;
    unsigned char *leftPix = image.data;
    unsigned char *rightPix = right.data;

    while(datasize--)
    {
    	leftPix[chRed] =
			CLAMP_LOW((int)leftPix[chRed] - (int)rightPix[chRed]);
    	leftPix[chGreen] =
			CLAMP_LOW((int)leftPix[chGreen] - (int)rightPix[chGreen]);
    	leftPix[chBlue] =
			CLAMP_LOW((int)leftPix[chBlue] - (int)rightPix[chBlue]);
        leftPix += 4;
		rightPix += 4;
	}
#else
  register int datasize = (image.xsize * image.ysize)>>1;
  register unsigned char *leftPix = image.data;
  register unsigned char *rightPix = right.data;

    while (datasize--) {
      SUB8(leftPix,rightPix);
      leftPix+=8;rightPix+=8;
    }
    MMXDONE;
#endif
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_subtract :: obj_setupCallback(t_class *)
{ }
