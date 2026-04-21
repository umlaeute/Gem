////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
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
//  mailto:zmoelnig@iem.at
//
/////////////////////////////////////////////////////////

// TODO add setroioffset and setroisize methods



// this is to paint easily your own pictures by passing a package of floats that contain all necessary image data)

#include "pix_set.h"
#include "Gem/State.h"

#include <string.h>
#include <stdio.h>

CPPEXTERN_NEW_WITH_TWO_ARGS(pix_set, t_floatarg, A_DEFFLOAT, t_floatarg,
                            A_DEFFLOAT);

/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_set :: pix_set(t_floatarg xsize, t_floatarg ysize)
  : m_mode(GL_RGBA)
  , m_pixels(NULL)
  , m_inputScale(255)
  , m_reqType(GL_UNSIGNED_BYTE)
{
  if (xsize < 1) {
    xsize = 256;
  }
  if (ysize < 1) {
    ysize = 256;
  }

  SETMess(xsize, ysize);

  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"),
            gensym("data"));
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
  gem::Rectangle*roi=NULL;
  state->get(GemState::getKey("pix.roi.rectangle"),roi);
  state->get(GemState::_PIX,m_pixels);
  if(roi) {
    m_roi=*roi;
    m_doROI=true;
  } else {
    m_doROI=false;
  }
  if(!m_pixels) {
    state->set(GemState::_PIX,&m_pixBlock);
  }
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
  state->set(GemState::_PIX, m_pixels);
}


/////////////////////////////////////////////////////////
// Template functions for different data types
//
/////////////////////////////////////////////////////////
namespace {
  template<typename T>
  void setPixelData(T* buffer, int argc, t_atom *argv, int mode, int csize, float inputScale, int& i, int roi_x1, int roi_x2, int roi_y1, int roi_y2, int xsize, bool m_doROI, pixBlock*pixels) {
    int picturesize = (roi_x2-roi_x1)*(roi_y2-roi_y1);
    int counter, n;

    switch (mode) {
    case GL_RGB:
      n = argc/3;
      counter=(picturesize<n)?picturesize:n;
      while (counter--) {
        if (std::is_same<T, unsigned char>::value) {
          buffer[chRed]   = static_cast<T>(inputScale*atom_getfloat(&argv[0])); // red
          buffer[chGreen] = static_cast<T>(inputScale*atom_getfloat(&argv[1])); // green
          buffer[chBlue]  = static_cast<T>(inputScale*atom_getfloat(&argv[2])); // blue
        } else {
          buffer[chRed]   = static_cast<T>(atom_getfloat(&argv[0])); // red
          buffer[chGreen] = static_cast<T>(atom_getfloat(&argv[1])); // green
          buffer[chBlue]  = static_cast<T>(atom_getfloat(&argv[2])); // blue
        }
        buffer[chAlpha] = static_cast<T>(0);                                 // alpha
        argv+=3;
        if (m_doROI) {
          i++;
          buffer = reinterpret_cast<T*>(pixels->image.data) + csize*(( i /
                   (roi_x2-roi_x1) + roi_y1 ) * pixels->image.xsize + (i %
                       (roi_x2-roi_x1)) + roi_x1) ;
        } else {
          buffer+=4;
        }
      }
      break;
    case GEM_GRAY:
      counter=(picturesize<argc)?picturesize:argc;
      if (csize == 4) {
        while (counter--) {
          if (std::is_same<T, unsigned char>::value) {
            buffer[chRed] = buffer[chGreen] = buffer[chBlue] = static_cast<T>(
                                                inputScale*atom_getfloat(argv));   // rgb
          } else {
            buffer[chRed] = buffer[chGreen] = buffer[chBlue] = static_cast<T>(atom_getfloat(argv));   // rgb
          }
          buffer[chAlpha] = static_cast<T>(0);                                    // alpha
          argv++;
          if (m_doROI) {
            i++;
            buffer = reinterpret_cast<T*>(pixels->image.data) + csize*(( i /
                     (roi_x2-roi_x1) + roi_y1 ) * pixels->image.xsize + (i %
                         (roi_x2-roi_x1)) + roi_x1) ;
          } else {
            buffer+=4;
          }
        }
      } else if (csize == 1) {
        while (counter--) {
          if (std::is_same<T, unsigned char>::value) {
            buffer[0] = static_cast<T>(inputScale*atom_getfloat(argv));
          } else {
            buffer[0] = static_cast<T>(atom_getfloat(argv));
          }
          argv++;
          if (m_doROI) {
            i++;
            buffer = reinterpret_cast<T*>(pixels->image.data) + csize*(( i /
                     (roi_x2-roi_x1) + roi_y1 ) * pixels->image.xsize + (i %
                         (roi_x2-roi_x1)) + roi_x1) ;
          } else {
            buffer++;
          }
        }
      }
      break;
    case GEM_YUV:
      // ?
      break;
    default:
      n = argc/4;
      counter=(picturesize<n)?picturesize:n;
      while (counter--) {
        if (std::is_same<T, unsigned char>::value) {
          buffer[chRed]   = static_cast<T>(inputScale*atom_getfloat(&argv[0])); // red
          buffer[chGreen] = static_cast<T>(inputScale*atom_getfloat(&argv[1])); // green
          buffer[chBlue]  = static_cast<T>(inputScale*atom_getfloat(&argv[2])); // blue
          buffer[chAlpha] = static_cast<T>(inputScale*atom_getfloat(&argv[3])); // alpha
        } else {
          buffer[chRed]   = static_cast<T>(atom_getfloat(&argv[0])); // red
          buffer[chGreen] = static_cast<T>(atom_getfloat(&argv[1])); // green
          buffer[chBlue]  = static_cast<T>(atom_getfloat(&argv[2])); // blue
          buffer[chAlpha] = static_cast<T>(atom_getfloat(&argv[3])); // alpha
        }
        argv+=4;
        if (m_doROI) {
          i++;
          buffer = reinterpret_cast<T*>(pixels->image.data) + csize*(( i /
                   (roi_x2-roi_x1) + roi_y1 ) * pixels->image.xsize + (i %
                       (roi_x2-roi_x1)) + roi_x1) ;
        } else {
          buffer+=4;
        }
      }
    }
  }
};

/////////////////////////////////////////////////////////
// DATAMess
//
/////////////////////////////////////////////////////////
void pix_set :: DATAMess(t_symbol* s, int argc, t_atom *argv)
{
  int i = 0;
  void *buffer;

  pixBlock*pixels=m_pixels?m_pixels:&m_pixBlock;

  int roi_x1=0;
  int roi_x2=pixels->image.xsize;
  int roi_y1=0;
  int roi_y2=pixels->image.ysize;

  if (!m_doROI) {
    // if no ROI is set, set whole image black before setting pixels values
    pixels->image.setBlack();
    buffer = pixels->image.data;
  } else {
    roi_x1=m_roi.x1*(0.5+pixels->image.xsize);
    roi_x2=m_roi.x2*(0.5+pixels->image.xsize);
    roi_y1=m_roi.y1*(0.5+pixels->image.ysize);
    roi_y2=m_roi.y2*(0.5+pixels->image.ysize);

    buffer = pixels->image.data + pixels->image.csize*(( i /
             (roi_x2-roi_x1) + roi_y1 ) * pixels->image.xsize + (i %
                 (roi_x2-roi_x1)) + roi_x1) ;
  }

  // Handle different data types using template functions
  switch(m_reqType) {
  case GL_UNSIGNED_BYTE: // BYTE mode
    setPixelData<unsigned char>(static_cast<unsigned char*>(buffer), argc, argv, m_mode,
                                pixels->image.csize, m_inputScale, i, roi_x1, roi_x2, roi_y1, roi_y2,
                                pixels->image.xsize, m_doROI, pixels);
    break;
  case GL_FLOAT:
    setPixelData<GLfloat>(static_cast<GLfloat*>(buffer), argc, argv, m_mode,
                          pixels->image.csize, m_inputScale, i, roi_x1, roi_x2, roi_y1, roi_y2,
                          pixels->image.xsize, m_doROI, pixels);
    break;
  case GL_DOUBLE:
    setPixelData<GLdouble>(static_cast<GLdouble*>(buffer), argc, argv, m_mode,
                           pixels->image.csize, m_inputScale, i, roi_x1, roi_x2, roi_y1, roi_y2,
                           pixels->image.xsize, m_doROI, pixels);
    break;
  default:
    setPixelData<unsigned char>(static_cast<unsigned char*>(buffer), argc, argv, m_mode,
                                pixels->image.csize, m_inputScale, i, roi_x1, roi_x2, roi_y1, roi_y2,
                                pixels->image.xsize, m_doROI, pixels);
  }
  pixels->newimage = true;
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
  m_mode = GEM_GRAY;
}

/////////////////////////////////////////////////////////
// SETMess
//
/////////////////////////////////////////////////////////
void pix_set :: SETMess(int xsize, int ysize)
{
  if ((xsize < 1) || (ysize < 1)) {
    return;
  }
  m_pixBlock.image.clear();
  m_pixBlock.image.xsize = (int)xsize;
  m_pixBlock.image.ysize = (int)ysize;
  m_pixBlock.image.setFormat(GEM_RGBA);
  m_pixBlock.image.reallocate();
  m_pixBlock.image.setBlack();
}

/////////////////////////////////////////////////////////
// Template function for filling pixel data
//
/////////////////////////////////////////////////////////
namespace {
  template<typename T>
  void fillPixelData(T* buffer, int argc, t_atom *argv, int mode, float inputScale, int& i, int roi_x1, int roi_x2, int roi_y1, int roi_y2, int xsize, bool m_doROI, pixBlock*pixels) {
    int picturesize = (roi_x2-roi_x1)*(roi_y2-roi_y1);
    int counter = picturesize;
    T r,g,b,a;

    switch (mode) {
    case GL_RGB:
      if ( argc==1 ) {
        r=g=b=a=static_cast<T>(inputScale*atom_getfloat(&argv[0]));
      } else if ( argc ==3 ) {
        r=static_cast<T>(inputScale*atom_getfloat(&argv[0]));
        g=static_cast<T>(inputScale*atom_getfloat(&argv[1]));
        b=static_cast<T>(inputScale*atom_getfloat(&argv[2]));
        a=static_cast<T>(0);
      } else {
        return; // error handled outside
      }
      while (counter--) {
        buffer[chRed]   = r; // red
        buffer[chGreen] = g; // green
        buffer[chBlue]  = b; // blue
        buffer[chAlpha] = a; // alpha
        if (m_doROI) {
          i++;
          buffer = reinterpret_cast<T*>(pixels->image.data) + 4*(( i /
                   (roi_x2-roi_x1) + roi_y1 ) * pixels->image.xsize + (i %
                       (roi_x2-roi_x1)) + roi_x1) ;
        } else {
          buffer+=4;
        }
      }
      break;
    case GEM_GRAY:
      if ( argc>0 ) {
        r=g=b=static_cast<T>(inputScale*atom_getfloat(&argv[0]));
        a=static_cast<T>(0);
      } else {
        return; // error handled outside
      }
      while (counter--) {
        buffer[chRed] = r;
        buffer[chGreen] = g;
        buffer[chBlue] = b;
        buffer[chAlpha] = a;
        if (m_doROI) {
          i++;
          buffer = reinterpret_cast<T*>(pixels->image.data) + 4*(( i /
                   (roi_x2-roi_x1) + roi_y1 ) * pixels->image.xsize + (i %
                       (roi_x2-roi_x1)) + roi_x1) ;
        } else {
          buffer+=4;
        }
      }
      break;
    case GEM_YUV:
      // ?
      break;
    default:
      if ( argc==1 ) {
        r=g=b=a=static_cast<T>(inputScale*atom_getfloat(&argv[0]));
      } else if ( argc == 4 ) {
        r=static_cast<T>(inputScale*atom_getfloat(&argv[0]));
        g=static_cast<T>(inputScale*atom_getfloat(&argv[1]));
        b=static_cast<T>(inputScale*atom_getfloat(&argv[2]));
        a=static_cast<T>(inputScale*atom_getfloat(&argv[3]));
      } else {
        return; // error handled outside
      }
      while (counter--) {
        buffer[chRed]   = r; // red
        buffer[chGreen] = g; // green
        buffer[chBlue]  = b; // blue
        buffer[chAlpha] = a; // alpha
        if (m_doROI) {
          i++;
          buffer = reinterpret_cast<T*>(pixels->image.data) + 4*(( i /
                   (roi_x2-roi_x1) + roi_y1 ) * pixels->image.xsize + (i %
                       (roi_x2-roi_x1)) + roi_x1) ;
        } else {
          buffer+=4;
        }
      }
    }
  }
};

/////////////////////////////////////////////////////////
// CLEARMess
//
/////////////////////////////////////////////////////////
void pix_set :: FILLMess(t_symbol* s, int argc, t_atom *argv)
{
  int i=0;
  void *buffer;

  pixBlock*pixels=m_pixels?m_pixels:&m_pixBlock;

  int roi_x1=0;
  int roi_x2=pixels->image.xsize;
  int roi_y1=0;
  int roi_y2=pixels->image.ysize;

  // Validate arguments before calling template functions
  switch (m_mode) {
  case GL_RGB:
    if ( argc != 1 && argc != 3 ) {
      error("fill need 1 or 3 float arg in RGB mode");
      return;
    }
    break;
  case GEM_GRAY:
    if ( argc <= 0 ) {
      error("fill need 1 float arg in GREY mode");
      return;
    }
    break;
  default:
    if ( argc != 1 && argc != 4 ) {
      error("fill need 1 or 4 float arg in RGBA mode");
      return;
    }
    break;
  }

  if (!m_doROI) {
    // if no ROI is set, set whole image black before setting pixels values
    pixels->image.setBlack();
    buffer = pixels->image.data;
  } else {
    roi_x1=m_roi.x1*(0.5+pixels->image.xsize);
    roi_x2=m_roi.x2*(0.5+pixels->image.xsize);
    roi_y1=m_roi.y1*(0.5+pixels->image.ysize);
    roi_y2=m_roi.y2*(0.5+pixels->image.ysize);

    buffer = pixels->image.data + pixels->image.csize*(( i /
             (roi_x2-roi_x1) + roi_y1 ) * pixels->image.xsize + (i %
                 (roi_x2-roi_x1)) + roi_x1) ;
  }

  // Handle different data types using template functions
  switch(m_reqType) {
  case GL_UNSIGNED_BYTE: // BYTE mode
    fillPixelData<unsigned char>(static_cast<unsigned char*>(buffer), argc, argv, m_mode,
                                 m_inputScale, i, roi_x1, roi_x2, roi_y1, roi_y2,
                                 pixels->image.xsize, m_doROI, pixels);
    break;
  case GL_FLOAT:
    fillPixelData<GLfloat>(static_cast<GLfloat*>(buffer), argc, argv, m_mode,
                           m_inputScale, i, roi_x1, roi_x2, roi_y1, roi_y2,
                           pixels->image.xsize, m_doROI, pixels);
    break;
  case GL_DOUBLE:
    fillPixelData<GLdouble>(static_cast<GLdouble*>(buffer), argc, argv, m_mode,
                            m_inputScale, i, roi_x1, roi_x2, roi_y1, roi_y2,
                            pixels->image.xsize, m_doROI, pixels);
    break;
  }
}

/////////////////////////////////////////////////////////
// BANGMess
//
/////////////////////////////////////////////////////////
void pix_set :: BANGMess(void)
{
  m_pixBlock.newimage = true;
}

/////////////////////////////////////////////////////////
// bytemodeMess
//
/////////////////////////////////////////////////////////
void pix_set :: bytemodeMess(bool v)
{
  m_inputScale=v ? 1. : 255.;
}

/////////////////////////////////////////////////////////
// typeMess
//
/////////////////////////////////////////////////////////
void pix_set :: typeMess(std::string type)
{
  if("BYTE" == type) {
    m_reqType = GL_UNSIGNED_BYTE;
  } else if ("FLOAT" == type) {
    m_reqType = GL_FLOAT;
  } else if ("DOUBLE" == type) {
    m_reqType = GL_DOUBLE;
  } else {
    error("invalid type '%s': must be 'BYTE', 'FLOAT' or 'DOUBLE'", type.c_str());
    m_reqType = 0;
    return;
  }
  if(m_reqType) {
    // Reallocate the image with new type
    if(GEM_RGBA == m_mode)
      m_pixBlock.image.setFormat(GL_RGBA);
    m_pixBlock.image.type = m_reqType;
    m_pixBlock.image.upsidedown = true; // Reset upsidedown for consistency
    m_pixBlock.image.reallocate();
    m_pixBlock.image.setBlack();
  }
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
  CPPEXTERN_MSG0(classPtr, "RGB", RGBMess);
  CPPEXTERN_MSG0(classPtr, "rgb", RGBMess);
  //  CPPEXTERN_MSG0(classPtr, "YUV" , YUVMess);
  //  CPPEXTERN_MSG0(classPtr, "yuv" , YUVMess);

  CPPEXTERN_MSG0(classPtr, "GREY", GREYMess);
  CPPEXTERN_MSG0(classPtr, "grey", GREYMess);
  CPPEXTERN_MSG0(classPtr, "GRAY", GREYMess);
  CPPEXTERN_MSG0(classPtr, "gray", GREYMess);

  CPPEXTERN_MSG (classPtr, "data", DATAMess);
  CPPEXTERN_MSG2(classPtr, "set", SETMess, int, int);
  CPPEXTERN_MSG (classPtr, "fill", FILLMess);
  CPPEXTERN_MSG0(classPtr, "bang", BANGMess);
  CPPEXTERN_MSG1(classPtr, "bytemode", bytemodeMess, bool);
  CPPEXTERN_MSG1(classPtr, "type", typeMess, std::string);
}
