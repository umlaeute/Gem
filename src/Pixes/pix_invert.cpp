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
void pix_invert :: processRGBAImage(imageStruct &image)
{
  int i = image.xsize * image.ysize;
    
  unsigned char *base = image.data;
  while (i) {
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
  while (i--) {
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

#ifdef ALTIVEC
    processYUVAltivec(image);
    return;
#else

  int h,w;
  long src;

  src = 0;

  //format is U Y V Y

  for (h=0; h<image.ysize; h++){
    for(w=0; w<image.xsize/2; w++){
      image.data[src] = 255 - image.data[src];
      image.data[src+1] = 255 - image.data[src+1];
      image.data[src+2] = 255 - image.data[src+2];
      image.data[src+3] = 255 - image.data[src+3];
      src+=4;
    }
  }
#endif
}

/////////////////////////////////////////////////////////
// processYUVAltivec  -- good stuff apply liberally
//
/////////////////////////////////////////////////////////
void pix_invert :: processYUVAltivec(imageStruct &image)
{
#ifdef ALTIVEC
int h,w,width;
//post("pix_invert: Altivec");
   width = image.xsize/8;

    union{
        unsigned char c[16];
        vector unsigned char v;
    }charBuffer;

    vector unsigned char offset;
    vector unsigned char *inData = (vector unsigned char*) image.data;
    
    charBuffer.c[0] = 255;
    offset = charBuffer.v;
    offset = (vector unsigned char) vec_splat(offset,0);
    
    UInt32			prefetchSize = GetPrefetchConstant( 16, 1, 256 );
	vec_dst( inData, prefetchSize, 0 );
        
    for ( h=0; h<image.ysize; h++){
        for (w=0; w<width; w++)
        {
        
	vec_dst( inData, prefetchSize, 0 );
        
        inData[0]=vec_subs(offset,inData[0]);
        inData++;
        
         }
        vec_dss( 0 );
    }  /*end of working altivec function */
    
#endif ALTIVEC
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_invert :: obj_setupCallback(t_class *)
{ }
