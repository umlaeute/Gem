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

#include "pix_add.h"

CPPEXTERN_NEW(pix_add)

/////////////////////////////////////////////////////////
//
// pix_add
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_add :: pix_add()
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_add :: ~pix_add()
{ }

/////////////////////////////////////////////////////////
// processDualImage
//
/////////////////////////////////////////////////////////
void pix_add :: processRGBA_RGBA(imageStruct &image, imageStruct &right)
{
#ifndef MMX
    int datasize = image.xsize * image.ysize;
    unsigned char *leftPix = image.data;
    unsigned char *rightPix = right.data;

    while(datasize--)
    {
    	leftPix[chRed] =
			CLAMP_HIGH((int)leftPix[chRed] + (int)rightPix[chRed]);
    	leftPix[chGreen] =
			CLAMP_HIGH((int)leftPix[chGreen] + (int)rightPix[chGreen]);
    	leftPix[chBlue] =
			CLAMP_HIGH((int)leftPix[chBlue] + (int)rightPix[chBlue]);
        leftPix += 4;
		rightPix += 4;
    }
#else
  register int datasize = (image.xsize * image.ysize)>>3;
  register unsigned char *leftPix = image.data;
  register unsigned char *rightPix = right.data;

  // Now the Alpha channel is added too, if this is good ?
  //MMXSTART;
  
  while (datasize--) {
    ADD8(leftPix,rightPix);
    leftPix+=8;rightPix+=8;
    ADD8(leftPix,rightPix);
    leftPix+=8;rightPix+=8;
    ADD8(leftPix,rightPix);
    leftPix+=8;rightPix+=8;
    ADD8(leftPix,rightPix);
    leftPix+=8;rightPix+=8;
  }
  MMXDONE;
#endif
}

/////////////////////////////////////////////////////////
// processDualGray
//
/////////////////////////////////////////////////////////
void pix_add :: processGray_Gray(imageStruct &image, imageStruct &right)
{
#ifndef MMX
    int datasize = image.xsize * image.ysize;
    unsigned char *leftPix = image.data;
    unsigned char *rightPix = right.data;

    while(datasize--) {
      leftPix[chGray] = CLAMP_HIGH((int)leftPix[chGray] + (int)rightPix[chGray]);
      leftPix++;
      rightPix++;
    }
#else
    int datasize = (image.xsize * image.ysize)>>2;
    MMXSTART;

    while (datasize--) {
      register unsigned char *leftPix = image.data;
      register unsigned char *rightPix = right.data;
      ADD8(leftPix,rightPix);
      leftPix+=8;rightPix+=8;
    }
    MMXDONE;
#endif
}

/////////////////////////////////////////////////////////
// processRightGray
//
/////////////////////////////////////////////////////////
void pix_add :: processRGBA_Gray(imageStruct &image, imageStruct &right)
{
  int datasize = image.xsize * image.ysize;
  unsigned char *leftPix = image.data;
  unsigned char *rightPix = right.data;
  
  while(datasize--)    {
    int alpha = rightPix[chGray];
    leftPix[chRed]   = CLAMP_HIGH((int)leftPix[chRed]   + alpha);
    leftPix[chGreen] = CLAMP_HIGH((int)leftPix[chGreen] + alpha);
    leftPix[chBlue]  = CLAMP_HIGH((int)leftPix[chBlue]  + alpha);
    leftPix += 4;
    rightPix++;
    }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_add :: obj_setupCallback(t_class *)
{ }
