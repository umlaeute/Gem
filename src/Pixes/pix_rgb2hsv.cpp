////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_rgb2hsv.h"

CPPEXTERN_NEW(pix_rgb2hsv)

/////////////////////////////////////////////////////////
//
// pix_rgb2hsv
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_rgb2hsv :: pix_rgb2hsv()
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_rgb2hsv :: ~pix_rgb2hsv()
{ }

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_rgb2hsv :: processRGBAImage(imageStruct &image)
{
  unsigned char *pixels = image.data;
  int count = image.ysize * image.xsize;
  while (count--)
    {
      unsigned char s, v;
      int h=0;
      unsigned char r=pixels[chRed], g=pixels[chGreen], b=pixels[chBlue];
      unsigned char max = TRI_MAX(r, g, b);
      unsigned char min = TRI_MIN(r, g, b);

      // value
      v = max;

      if (max==min) h = s = 0;
      else {
	float delta_inv = 255./(float)(max - min);
	delta_inv = 42.5/(float)(max - min);
	s = (max - min) * 255 / max;
	if      (r==max)h=    (unsigned char)((g - b) * delta_inv);// between magenta and cyan
	else if (g==max)h= 85+(unsigned char)((b - r) * delta_inv);// between yellow and magenta
	else if (b==max)h=170+(unsigned char)((r - g) * delta_inv);// between cyan and yellow

      }

      //      post("r=%d g=%d b=%d", r, g, b);
      //      post("h=%d s=%d v=%d", h, s, v);
      if (h<0)h+=255;

      pixels[chRed] = h%255;
      pixels[chGreen]=s;
      pixels[chBlue] =v;
      pixels += 4;
    }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_rgb2hsv :: obj_setupCallback(t_class *)
{ }
