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
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_hsv2rgb.h"

CPPEXTERN_NEW(pix_hsv2rgb)

  /////////////////////////////////////////////////////////
//
// pix_hsv2rgb
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_hsv2rgb :: pix_hsv2rgb()
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_hsv2rgb :: ~pix_hsv2rgb()
{ }

///////////////////////////////////////
// Utilities
//
///////////////////////////////////////

static inline unsigned char TRI_MAX(unsigned char v1, unsigned char v2, unsigned char v3){
  if (v1 > v2 && v1 > v3) return(v1);
  if (v2 > v3) return(v2);
  return(v3);
}

static inline unsigned char TRI_MIN(unsigned char v1, unsigned char v2, unsigned char v3){
  if (v1 < v2 && v1 < v3) return(v1);
  if (v2 < v3) return(v2);
  return(v3);
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_hsv2rgb :: processImage(imageStruct &image)
{
  unsigned char *pixels = image.data;
  int count = image.ysize * image.xsize;
  while (count--)
    {
      unsigned char r, b, g;
      unsigned char h = pixels[chRed], s=pixels[chGreen], v=pixels[chBlue];

      if (s) {
	int   i = (int)(h/42.5);
	float f = ((float)h/42.5)-i;

	float vf = (float)v/255.;
	float vs = vf*s;
	float vsf= vs*f;
	unsigned char p = (unsigned char)(v - vs);
	unsigned char q = (unsigned char)(v - vsf);
	unsigned char t = (unsigned char)(p + vsf);
	
	switch (i)
	  {
	  case 1:
	    r = q; g = v; b = p;
	    break;
	  case 2:
	    r = p; g = v; b = t;
	    break;
	  case 3:
	    r = p; g = q; b = v;
	    break;
	  case 4:
	    r = t; g = p; b = v;
	    break;
	  case 5:
	    r = v; g = p; b = q;
	    break;
	  default:
	    r = v; g = t; b = p;
	    break;
	  }

      } else r=g=b=v;
      //      post("pix    : r=%d g=%d b=%d", r, g, b);
      
      pixels[chRed] = r;
      pixels[chGreen]=g;
      pixels[chBlue] =b;
      pixels += 4;
    }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_hsv2rgb :: obj_setupCallback(t_class *)
{ }
