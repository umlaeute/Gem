////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_2grey.h"

CPPEXTERN_NEW(pix_2grey)

/////////////////////////////////////////////////////////
//
// pix_2grey
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_2grey :: pix_2grey()
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_2grey :: ~pix_2grey()
{ }

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_2grey :: processRGBAImage(imageStruct &image)
{
  unsigned char *pixels = image.data;
  int count = image.ysize * image.xsize;
  post("pix_2grey:: start processing %d pixels @ %x", count, pixels);
  post("*pixels=%d", *pixels);
  post("red0  =%d", pixels[chRed]);
  post("green0=%d", pixels[chGreen]);
  post("blue0 =%d", pixels[chBlue]);

  while (count--)    {
    //    post("count=%d", count);
    float grey = pixels[chRed] * 0.3086f + pixels[chGreen] * 0.6094f
      + pixels[chBlue] * 0.0820f;
    //post(".");
    pixels[chRed] = pixels[chGreen] = pixels[chBlue] = (unsigned char)grey;
    //post("..");
    pixels += 4;
    //post("...");
    }
  post("pix_2grey: done");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_2grey :: obj_setupCallback(t_class *)
{ }
