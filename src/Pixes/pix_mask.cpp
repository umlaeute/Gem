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

#include "pix_mask.h"

CPPEXTERN_NEW(pix_mask)

/////////////////////////////////////////////////////////
//
// pix_mask
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_mask :: pix_mask()
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_mask :: ~pix_mask()
{ }

/////////////////////////////////////////////////////////
// processDualImage
//
/////////////////////////////////////////////////////////
void pix_mask :: processDualImage(imageStruct &image, imageStruct &right)
{
    int datasize = image.xsize * image.ysize;
	unsigned char *pixels = image.data;
	unsigned char *mask = right.data;

	while (datasize--)
	{
    	// do we use the alpha value
    	pixels[chAlpha] = (	(int)mask[chRed] +
                                (int)mask[chGreen] +
                                (int)mask[chBlue] ) / 3;
		pixels += 4;
		mask += 4;
	}
}

/////////////////////////////////////////////////////////
// processRightGray
//
/////////////////////////////////////////////////////////
void pix_mask :: processRightGray(imageStruct &image, imageStruct &right)
{
    int datasize = image.xsize * image.ysize;
	unsigned char *pixels = image.data;
	unsigned char *mask = right.data;

	while(datasize--)
	{
   		pixels[chAlpha] = *mask;
		pixels += 4;
		mask++;
	}
}

/////////////////////////////////////////////////////////
// processDualYUV
//
/////////////////////////////////////////////////////////
void pix_mask :: processDualYUV(imageStruct &image, imageStruct &right)
{
    post("pix_mask:  YUV not yet implemented :-P");
}

/////////////////////////////////////////////////////////
// processRightYUV
//
/////////////////////////////////////////////////////////
void pix_mask :: processRightYUV(imageStruct &image, imageStruct &right)
{
    post("pix_mask:  YUV not yet implemented :-P");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_mask :: obj_setupCallback(t_class *)
{ }
