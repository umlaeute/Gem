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
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
//  pix_set
//
//  0409:forum::für::umläute:2000
//  IOhannes m zmoelnig
//  mailto:zmoelnig@iem.kug.ac.at
//
/////////////////////////////////////////////////////////



// this is to paint easily your own pictures by passing a package of floats that contain all necessary image data)

#include "pix_set.h"
#include "Gem/State.h"

#include <string.h>

CPPEXTERN_NEW_WITH_TWO_ARGS(pix_set, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_set :: pix_set(t_floatarg xsize, t_floatarg ysize) :
  m_mode(GL_RGBA)
{
  if (xsize < 1) xsize = 256;
  if (ysize < 1) ysize = 256;

  SETMess(xsize, ysize);

  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("data"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_set :: ~pix_set()
{
  cleanPixBlock();
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_set :: render(GemState *state)
{
  state->set(GemState::_PIX,&m_pixBlock);
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_set :: startRendering()
{
    m_pixBlock.newimage = true;
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_set :: postrender(GemState *state)
{
    m_pixBlock.newimage = false;
    //state->image = NULL;
}


/////////////////////////////////////////////////////////
// DATAMess
//
/////////////////////////////////////////////////////////
void pix_set :: DATAMess(t_symbol*s, int argc, t_atom *argv)
{
  m_pixBlock.image.setBlack();

  int picturesize = m_pixBlock.image.xsize * m_pixBlock.image.ysize, counter, n;
  unsigned char *buffer = m_pixBlock.image.data;


  switch (m_mode) {
  case GL_RGB:
    n = argc/3;
    counter=(picturesize<n)?picturesize:n;
    while (counter--) {
      buffer[chRed]   = (unsigned char)(255.*atom_getfloat(&argv[0])); // red
      buffer[chGreen] = (unsigned char)(255.*atom_getfloat(&argv[1])); // green
      buffer[chBlue]  = (unsigned char)(255.*atom_getfloat(&argv[2])); // blue
      buffer[chAlpha] = 0;					     // alpha
      argv+=3; buffer+=4;
    }
    break;
  case GL_LUMINANCE:
    counter=(picturesize<argc)?picturesize:argc;
    while (counter--) {
      buffer[chRed] = buffer[chGreen] = buffer[chBlue] = (unsigned char)(255.*atom_getfloat(argv));	// rgb
      buffer[chAlpha] = 0;									// alpha
      argv++;	buffer+=4;
    }
    break;
  case GL_YCBCR_422_GEM:
    // ?
    break;
  default:
    n = argc/4;
    counter=(picturesize<n)?picturesize:n;
    while (counter--) {
      buffer[chRed]   = (unsigned char)(255.*atom_getfloat(&argv[0])); // red
      buffer[chGreen] = (unsigned char)(255.*atom_getfloat(&argv[1])); // green
      buffer[chBlue]  = (unsigned char)(255.*atom_getfloat(&argv[2])); // blue
      buffer[chAlpha] = (unsigned char)(255.*atom_getfloat(&argv[3])); // alpha
      argv+=4; buffer+=4;
    }
  }
  m_pixBlock.newimage = true;
}



/////////////////////////////////////////////////////////
// RGBAMess
//
/////////////////////////////////////////////////////////
void pix_set :: RGBAMess(void)
{
	m_mode = GL_RGBA;
}
/////////////////////////////////////////////////////////
// RGBMess
//
/////////////////////////////////////////////////////////
void pix_set :: RGBMess(void)
{
	m_mode = GL_RGB;
}
/////////////////////////////////////////////////////////
// GREYMess
//
/////////////////////////////////////////////////////////
void pix_set :: GREYMess(void)
{
	m_mode = GL_LUMINANCE;
}

/////////////////////////////////////////////////////////
// SETMess
//
/////////////////////////////////////////////////////////
void pix_set :: SETMess(int xsize, int ysize)
{
	if ((xsize < 1) || (ysize < 1)) return;
	m_pixBlock.image.clear();
	m_pixBlock.image.xsize = (int)xsize;
	m_pixBlock.image.ysize = (int)ysize;
  m_pixBlock.image.setCsizeByFormat(GL_RGBA_GEM);
  m_pixBlock.image.reallocate();
  m_pixBlock.image.setBlack();
}

/////////////////////////////////////////////////////////
// cleanPixBlock -- free the pixel buffer memory
//
/////////////////////////////////////////////////////////
void pix_set :: cleanPixBlock()
{
  m_pixBlock.image.clear();
  m_pixBlock.image.data = NULL;
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_set :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG0(classPtr, "RGBA", RGBAMess);
  CPPEXTERN_MSG0(classPtr, "rgba", RGBAMess);
  CPPEXTERN_MSG0(classPtr, "RGB" , RGBMess);
  CPPEXTERN_MSG0(classPtr, "rgb" , RGBMess);
  //  CPPEXTERN_MSG0(classPtr, "YUV" , YUVMess);
  //  CPPEXTERN_MSG0(classPtr, "yuv" , YUVMess);

  CPPEXTERN_MSG0(classPtr, "GREY", GREYMess);
  CPPEXTERN_MSG0(classPtr, "grey", GREYMess);
  CPPEXTERN_MSG0(classPtr, "GRAY", GREYMess);
  CPPEXTERN_MSG0(classPtr, "gray", GREYMess);

  CPPEXTERN_MSG (classPtr, "data", DATAMess);
  CPPEXTERN_MSG2(classPtr, "set", SETMess, int, int);
}
