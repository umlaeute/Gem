////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyleft  (l) 2001 IOhannes m zmölnig
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_test.h"

CPPEXTERN_NEW(pix_test)

/////////////////////////////////////////////////////////
//
// pix_test
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_test :: pix_test()
{
  myImage.xsize=myImage.ysize=myImage.csize=1;
  myImage.allocate(1);
 }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_test :: ~pix_test()
{ }

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_test :: processRGBAImage(imageStruct &image)
{
    unsigned char *pixels = image.data;
    int count = image.ysize * image.xsize;
    unsigned char *newPix;

    orgImage = &image;
    orgdata  = image.data;

    if (myImage.xsize*myImage.ysize*myImage.csize != image.xsize*image.ysize*image.csize){
      int dataSize = image.xsize * image.ysize * image.csize;
      myImage.clear();
      myImage.allocate(dataSize);
    }
    myImage.xsize = image.xsize;
    myImage.ysize = image.ysize;
    myImage.csize = image.csize;
    myImage.type  = image.type;


    newPix = myImage.data+count*image.csize;
   
    while (count--) {
      newPix[chRed]  =pixels[chRed];
      newPix[chGreen]=pixels[chGreen];
      newPix[chBlue] =pixels[chBlue];
      newPix[chAlpha]=pixels[chAlpha];

      pixels+=4;
      newPix-=4;
    }

    image.data=myImage.data;
}

/////////////////////////////////////////////////////////
// processYUVImage
//
/////////////////////////////////////////////////////////
void pix_test :: processYUVImage(imageStruct &image)
{
    post("pix_test: YUV not yet implemented :-(");
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_test :: postrender(GemState *state)
{
  if (orgImage)orgImage->data = orgdata;
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_test :: obj_setupCallback(t_class *)
{ }
