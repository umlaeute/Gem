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
void pix_mask :: processRGBA_RGBA(imageStruct &image, imageStruct &right)
{
  int datasize = image.xsize * image.ysize;
  unsigned char *pixels = image.data;
  unsigned char *mask = right.data;

  while (datasize--)	{
    // do we use the alpha value
    pixels[chAlpha] = (mask[chRed  ] * 79  + 
		       mask[chGreen] * 156 +
		       mask[chBlue ] * 21)>>8;
    pixels += 4;
    mask += 4;
  }
}

/////////////////////////////////////////////////////////
// processRightGray
//
/////////////////////////////////////////////////////////
void pix_mask :: processRGBA_Gray(imageStruct &image, imageStruct &right)
{
  int datasize = image.xsize * image.ysize;
  unsigned char *pixels = image.data;
  unsigned char *mask = right.data;

  while(datasize--)	{
    pixels[chAlpha] = *mask++;
    pixels += 4;
  }
}

/////////////////////////////////////////////////////////
// processRightGray
//
/////////////////////////////////////////////////////////
void pix_mask :: processRGBA_YUV(imageStruct &image, imageStruct &right)
{
  int datasize = (image.xsize * image.ysize)>>1;
  unsigned char *pixels = image.data;
  unsigned char *mask = right.data;

  while(datasize--)	{
    pixels[chAlpha] = mask[chY0];
    pixels += 4;
    pixels[chAlpha] = mask[chY1];
    pixels += 4;
    mask   += 4;


  }
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_mask :: obj_setupCallback(t_class *)
{ }
