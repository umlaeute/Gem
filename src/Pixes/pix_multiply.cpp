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

#include "pix_multiply.h"

CPPEXTERN_NEW(pix_multiply)

/////////////////////////////////////////////////////////
//
// pix_multiply
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_multiply :: pix_multiply()
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_multiply :: ~pix_multiply()
{ }

/////////////////////////////////////////////////////////
// processDualImage
//
/////////////////////////////////////////////////////////
void pix_multiply :: processDualImage(imageStruct &image, imageStruct &right)
{
    int datasize = image.xsize * image.ysize;
    unsigned char *leftPix = image.data;
    unsigned char *rightPix = right.data;

    while(datasize--)
    {
    	leftPix[chRed] = INT_MULT(leftPix[chRed], rightPix[chRed]);
    	leftPix[chGreen] = INT_MULT(leftPix[chGreen], rightPix[chGreen]);
    	leftPix[chBlue] = INT_MULT(leftPix[chBlue], rightPix[chBlue]);
        leftPix += 4;
		rightPix += 4;
    }
}

/////////////////////////////////////////////////////////
// processRightGray
//
/////////////////////////////////////////////////////////
void pix_multiply :: processRightGray(imageStruct &image, imageStruct &right)
{
    int datasize = image.xsize * image.ysize;
    unsigned char *leftPix = image.data;
    unsigned char *rightPix = right.data;

    while(datasize--)
	{
		unsigned int alpha = rightPix[chGray];
    	leftPix[chRed] = INT_MULT(leftPix[chRed], alpha);
    	leftPix[chGreen] = INT_MULT(leftPix[chGreen], alpha);
    	leftPix[chBlue] = INT_MULT(leftPix[chBlue], alpha);
        leftPix += 4;
		rightPix++;
	}
}

/////////////////////////////////////////////////////////
// processDualGray
//
/////////////////////////////////////////////////////////
void pix_multiply :: processDualGray(imageStruct &image, imageStruct &right)
{
    int datasize = image.xsize * image.ysize;
    unsigned char *leftPix = image.data;
    unsigned char *rightPix = right.data;

    while(datasize--)
	{
		unsigned int alpha = rightPix[chGray];
    	leftPix[chGray] = INT_MULT(leftPix[chGray], alpha);
        leftPix++;
		rightPix++;
	}
}

/////////////////////////////////////////////////////////
// processDualYUV
//
/////////////////////////////////////////////////////////
void pix_multiply :: processDualYUV(imageStruct &image, imageStruct &right)
{
    post("pix_multiply: YUV not yet implemented :-(");
}

/////////////////////////////////////////////////////////
// processRightYUV
//
/////////////////////////////////////////////////////////
void pix_multiply :: processRightYUV(imageStruct &image, imageStruct &right)
{
    post("pix_multiply: YUV not yet implemented :-(");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_multiply :: obj_setupCallback(t_class *)
{ }
