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
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_composite.h"

CPPEXTERN_NEW(pix_composite)

/////////////////////////////////////////////////////////
//
// pix_composite
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_composite :: pix_composite()
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_composite :: ~pix_composite()
{
}

/////////////////////////////////////////////////////////
// processDualImage
//
/////////////////////////////////////////////////////////
void pix_composite :: processDualImage(imageStruct &image, imageStruct &right)
{
    int datasize = image.xsize * image.ysize;
	unsigned int alpha;

	// The src1, src2, dst is a little bit backwards.  This
	//	is because we want the image on the left inlet to be
	//	on top of the image on the right inlet.
	unsigned char *dst = image.data;
	unsigned char *src1 = right.data;
	unsigned char *src2 = image.data;

    while(datasize--)
    {
    	if ( (alpha = src2[chAlpha]) )
    	{
			if (alpha == 255)
			{
    			dst[chRed]   = src2[chRed];
    			dst[chGreen] = src2[chGreen];
    			dst[chBlue]  = src2[chBlue];
			}
			else
			{
				dst[chRed]   = INT_LERP(src1[chRed], src2[chRed], alpha);
				dst[chGreen] = INT_LERP(src1[chGreen], src2[chGreen], alpha);
				dst[chBlue]  = INT_LERP(src1[chBlue], src2[chBlue], alpha);
			}
    	}
		else
		{
			dst[chRed]		 = src1[chRed];
			dst[chGreen]	 = src1[chGreen];
			dst[chBlue]		 = src1[chBlue];
		}
		src1 += 4;
		src2 += 4;
		dst += 4;
    }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_composite :: obj_setupCallback(t_class *)
{ }
