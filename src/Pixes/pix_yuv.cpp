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

#include "pix_yuv.h"

CPPEXTERN_NEW(pix_yuv)

/////////////////////////////////////////////////////////
//
// pix_yuv
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_yuv :: pix_yuv()
{
  m_data = NULL;
  m_size = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_yuv :: ~pix_yuv()
{
  if (m_data) delete [] m_data;
}

void pix_yuv :: render(GemState *state) {
  GemPixObj::render(state);
}

void pix_yuv :: postrender(GemState *state) {
  GemPixObj::postrender(state);
}
/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_yuv :: processImage(imageStruct &image)
{
  //post("YUV: converting %x from %X", &image, image.format);
  if (image.format==GL_YCBCR_422_GEM)return;
  unsigned char *old_pix, *new_pix;
  int count = image.xsize*image.ysize;
  int format = image.format;

  if (image.xsize*image.ysize != m_size){
    delete [] m_data;
    m_size = image.xsize*image.ysize;
    m_data = new unsigned char[m_size*2]; // 2 is the size of the YUV_pixel
    //post("made buffer of size %d at %X", m_size, m_data);
  }

  old_pix = image.data;
  new_pix = m_data;

  //post("now doing the conversion from %X to %X", old_pix, new_pix);

  switch (format){
  case GL_YCBCR_422_GEM: /* nothing to do */
    break;
  case GL_RGBA:
    /*
    Y = 0.299*R + 0.587*G + 0.114*B;
    U = -0.169*R - 0.331*G + 0.500*B + 128.0;
    V = 0.500*R - 0.419*G - 0.081*B + 128.0;

    R = Y + (1.4075 * (V - 128));
    G = Y - (0.3455 * (U - 128)) - (0.7169 * (V - 128));
    B = Y + (1.7790 * (U - 128));
	     
    // we store data like this
    // UYVY
    // 8-bit:  U Y0 V Y1
    // U0=U1:=U; V0=V1:=V;
    */
    unsigned char r0, g0, b0, r1, g1, b1;
    while (count-=2){
      //post("reading...%d", count);
      r0 =old_pix[chRed];
      g0 =old_pix[chGreen];
      b0 =old_pix[chBlue];
      //post("read2");
      old_pix+=4;
      r1 =old_pix[chRed];
      g1 =old_pix[chGreen];
      b1 =old_pix[chBlue];
      old_pix+=4;

      //post("rgb: %d %d %d", r0, g0, b0);
      //post("rgb: %d %d %d", r1, g1, b1);
      //post("writing");
      register int x = (int)(-0.169*(r0) - 0.331*(g0) + 0.500*(b0) + 128.0); // U
      *new_pix++ = CLAMP(x);
      x = (int)(0.299*r0 + 0.587*g0 + 0.114*b0); // Y
      *new_pix++ = CLAMP(x);
      x = (int)(0.500*(r0) - 0.419*(g0) - 0.081*(b0) + 128.0); // V
      *new_pix++ = CLAMP(x);
      x = (int)(0.299*r1 + 0.587*g1 + 0.114*b1); // Y
      *new_pix++ = CLAMP(x);
    }
    new_pix-=4;
    //    post("YUV: %d %d %d %d", new_pix[0], new_pix[1], new_pix[2], new_pix[3]); 

    break;
  default:
    error("no method for this format !!!");
    post("if you know how to convert this format (%X) to YUV,\n"
	 "please contact the authors of this software", format);
    return;
  }

  image.data   = m_data;
  //  image.notowned = 1;
  image.format = GL_YCBCR_422_GEM;
  image.csize  = 2;
  //  post("yuv: %d %d %d\t%X %x", image.xsize, image.ysize, image.csize, image.format, image.data);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_yuv :: obj_setupCallback(t_class *classPtr)
{}
