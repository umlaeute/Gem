////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
// this is based on EffecTV by Fukuchi Kentarou
// * Copyright (C) 2001 FUKUCHI Kentarou
//
/////////////////////////////////////////////////////////

#include "pix_rgba.h"

CPPEXTERN_NEW(pix_rgba)

/////////////////////////////////////////////////////////
//
// pix_rgba
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_rgba :: pix_rgba()
{
  m_data = NULL;
  m_size = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_rgba :: ~pix_rgba()
{
  if (m_data) delete [] m_data;
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_rgba :: processImage(imageStruct &image)
{
//  post("RGBA processing");
  if (image.format==GL_RGBA)return;
  unsigned char *old_pix, *new_pix;
  int count = image.xsize*image.ysize;
  int format = image.format;

  if (image.xsize*image.ysize != m_size){
    delete [] m_data;
    m_size = image.xsize*image.ysize;
    m_data = new unsigned char[m_size*4];
  }

  old_pix = image.data;
  new_pix = m_data;

  switch (format){
  case GL_RGBA: /* nothing to do */
    break;
  case GL_RGB:  
    while(count--){
      new_pix[chRed]  = old_pix[0];
      new_pix[chGreen]= old_pix[1];
      new_pix[chBlue] = old_pix[2];
      new_pix[chAlpha]= 255;
      new_pix+=4;
      old_pix+=3;
    }
    break;
  case GL_BGR_EXT: 
    while(count--){
      new_pix[chRed]  =old_pix[2];
      new_pix[chGreen]=old_pix[1];
      new_pix[chBlue] =old_pix[0];
      new_pix[chAlpha]=255;
      old_pix+=3;
      new_pix+=4;
    }
    break;
  case GL_BGRA_EXT:
  #ifndef MACOSX  //bypassed on OSX since this is the correct format
    while(count--){
      new_pix[chRed]  =old_pix[2];
      new_pix[chGreen]=old_pix[1];
      new_pix[chBlue] =old_pix[0];
      new_pix[chAlpha]=old_pix[3];
      old_pix+=4;
      new_pix+=4;
    }
    #endif //MACOSX
    break;
    
  case GL_LUMINANCE:
    while(count--){
      new_pix[chRed]  =old_pix[0];
      new_pix[chGreen]=old_pix[0];
      new_pix[chBlue] =old_pix[0];
      new_pix[chAlpha]=255;
      old_pix++;
      new_pix+=4;
    }
    break;
  case GL_YCBCR_422_GEM: // YUV
    // i was amazed, this really works (but is - of course - very slow)
    int Y, U, V, R, G, B;
    R=255; G=0; B=0;

    Y = (int)(0.299*R + 0.587*G + 0.114*B);
    U = (int)(-0.169*R - 0.331*G + 0.500*B + 128.0);
    V = (int)(0.500*R - 0.419*G - 0.081*B + 128.0);
    /*
    R = Y + (1.4075 * (V - 128));
    G = Y - (0.3455 * (U - 128)) - (0.7169 * (V - 128));
    B = Y + (1.7790 * (U - 128));
    */


    R = (int)(1 * Y -  0.0009267*(U-128)  + 1.4016868*(V-128));
    G = (int)(1 * Y -  0.3436954*(U-128)  - 0.7141690*(V-128));
    B = (int)(1 * Y +  1.7721604*(U-128)  + 0.0009902*(V-128));

    //    post("YUV: %d %d %d", Y, U, V);
    //    post("RGB: %d %d %d", R, G, B);
    /*
    // we store data like this
    // UYVY
    // 8-bit:  U Y0 V Y1
    // U0=U1:=U; V0=V1:=V;
    */
    unsigned char y0, y1, u, v;
    register int x;
    while (count-=2){
      u =*old_pix++;
      y0=*old_pix++;
      v =*old_pix++;
      y1=*old_pix++;
      x = (int)(y0 + (1.4075 * (v - 128)));
      new_pix[chRed]  = CLAMP(x);
      x = (int)(y0 - (0.3455 * (u - 128)) - (0.7169 * (v - 128)));
      new_pix[chGreen]= CLAMP(x);
      x = (int)(y0 + (1.7790 * (u - 128)));
      new_pix[chBlue] = CLAMP(x);
      new_pix[chAlpha]=255;     
      new_pix+=4;
      x = (int)(y1 + (1.4075 * (v - 128)));
      new_pix[chRed]  = CLAMP(x);
      x = (int)(y1 - (0.3455 * (u - 128)) - (0.7169 * (v - 128)));
      new_pix[chGreen]= CLAMP(x);
      x = (int)(y1 + (1.7790 * (u - 128)));
      new_pix[chBlue] = CLAMP(x);
      new_pix[chAlpha]=255;     
      new_pix+=4;
    }
    new_pix-=4;
    //    post("RGBA: %d %d%d %d", new_pix[0], new_pix[1],new_pix[2], new_pix[3]); 
    break;
  default:
    post("no method for this format !!!");
    post("if you know how to convert this format (%X) to RGBA,\n"
	 "please contact the authors of this software", format);
    return;
  }

  image.data   = m_data;
  image.notowned = 0;
  #ifndef MACOSX  //keeps this object from messing up display on OSX
  image.format = GL_RGBA;
  #endif
  image.csize  = 4;
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_rgba :: obj_setupCallback(t_class *classPtr)
{
}
