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

#include "pix_takealpha.h"

CPPEXTERN_NEW(pix_takealpha)

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
void pix_takealpha :: processDualImage(imageStruct &image, imageStruct &right)
{
    int datasize = image.xsize * image.ysize;
    unsigned char *leftPix = image.data;
    unsigned char *rightPix = right.data;

    while(datasize--)
    {
    	leftPix[chAlpha] =rightPix[chAlpha];
        leftPix += 4;
	rightPix += 4;
    }
}

/////////////////////////////////////////////////////////
// processDualGray
//
/////////////////////////////////////////////////////////
void pix_takealpha :: processDualGray(imageStruct &image, imageStruct &right)
{

}

/////////////////////////////////////////////////////////
// processRightGray
//
/////////////////////////////////////////////////////////
void pix_takealpha :: processRightGray(imageStruct &image, imageStruct &right)
{
    int datasize = image.xsize * image.ysize;
    unsigned char *leftPix = image.data;
    unsigned char *rightPix = right.data;

    while(datasize--)
    {
      leftPix[chAlpha] = rightPix[chGray];
      leftPix += 4;
      rightPix+=4;
    }
}

/////////////////////////////////////////////////////////
// processDualYUV
//
/////////////////////////////////////////////////////////
void pix_takealpha :: processDualYUV(imageStruct &image, imageStruct &right)
{
    post("pix_takealpha: YUV not yet implemented :-(");
}

/////////////////////////////////////////////////////////
// processRightYUV
//
/////////////////////////////////////////////////////////
void pix_takealpha :: processRightYUV(imageStruct &image, imageStruct &right)
{
    post("pix_takealpha: YUV not yet implemented :-(");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_takealpha :: obj_setupCallback(t_class *)
{ }
