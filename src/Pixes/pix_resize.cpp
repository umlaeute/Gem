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

#include "pix_resize.h"

#include <math.h>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif // __APPLE__

CPPEXTERN_NEW(pix_resize)

/////////////////////////////////////////////////////////
//
// pix_resize
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_resize :: pix_resize()
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_resize :: ~pix_resize()
{ }

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_resize :: processImage(imageStruct &image)
{
    // do we need to resize the image?
    // need to check if dimensions are a power of two 
    int wN, hN;

    wN = powerOfTwo(image.xsize);
    hN = powerOfTwo(image.ysize);

    if (wN != image.xsize || hN != image.ysize)
    {
      GLint gluError;
      unsigned char *resizedData = new unsigned char 
	[wN * hN * image.csize];
      
      gluError = gluScaleImage(image.format,
			       image.xsize, image.ysize,
			       image.type, image.data,
			       wN, hN,
			       image.type, resizedData);
      if ( gluError )
	{
	  post("gluError: %d", gluError);
	  post("unable to resize image");
	  if(resizedData)delete [] resizedData;resizedData=NULL;
	  return;
	}
      //      image.clear();
      image.data = resizedData;
      image.xsize = wN;
      image.ysize = hN;
    }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_resize :: obj_setupCallback(t_class *)
{ }
