////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
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
void pix_add :: processDualImage(imageStruct &image, imageStruct &right)
{
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
}

/////////////////////////////////////////////////////////
// processDualGray
//
/////////////////////////////////////////////////////////
void pix_add :: processDualGray(imageStruct &image, imageStruct &right)
{
    int datasize = (image.xsize * image.ysize)>>2;
    MMXSTART;

    while (datasize--) {
      register unsigned char *leftPix = image.data;
      register unsigned char *rightPix = right.data;
      ADD8(leftPix,rightPix);
      leftPix+=8;rightPix+=8;
    }
    MMXDONE;
}

/////////////////////////////////////////////////////////
// processRightGray
//
/////////////////////////////////////////////////////////
void pix_add :: processRightGray(imageStruct &image, imageStruct &right)
{
    int datasize = image.xsize * image.ysize;
    unsigned char *leftPix = image.data;
    unsigned char *rightPix = right.data;

    while(datasize--)
    {
		int alpha = rightPix[chGray];
    	leftPix[chRed] =
			CLAMP_HIGH((int)leftPix[chRed] + alpha);
    	leftPix[chGreen] =
			CLAMP_HIGH((int)leftPix[chGreen] + alpha);
    	leftPix[chBlue] =
			CLAMP_HIGH((int)leftPix[chBlue] + alpha);
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
