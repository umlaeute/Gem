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

#include "pix_invert.h"

CPPEXTERN_NEW(pix_invert)

/////////////////////////////////////////////////////////
//
// pix_invert
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_invert :: pix_invert()
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_invert :: ~pix_invert()
{ }
 
/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_invert :: processImage(imageStruct &image)
{
    int i = image.xsize * image.ysize;
    
    unsigned char *base = image.data;
	while (i)
    {
		i--;
        unsigned char alpha = base[chAlpha];
		*((unsigned long *)base) = ~*((unsigned long *)base);
        base[chAlpha] = alpha;
        base += 4;
    }
}

/////////////////////////////////////////////////////////
// processGrayImage
//
/////////////////////////////////////////////////////////
void pix_invert :: processGrayImage(imageStruct &image)
{
    int i = image.xsize * image.ysize;
    
    unsigned char *base = image.data;
	while (i--)
    {
        base[chGray] = 255 - base[chGray];
        base++;
    }    
}

/////////////////////////////////////////////////////////
// processYUVImage
//
/////////////////////////////////////////////////////////
void pix_invert :: processYUVImage(imageStruct &image)
{
    post("pix_invert: YUV not yet implemented :-(");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_invert :: obj_setupCallback(t_class *)
{ }
