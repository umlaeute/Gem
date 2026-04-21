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
  void setPixelData(imageStruct&img, int argc, t_atom *argv,
                    const int mode, const float inputScale,
                    const int roi_x1, const int roi_x2, const int roi_y1, const int roi_y2, const bool doROI) {
    const int csize = img.csize, xsize = img.xsize;
    const int roi_dx = roi_x2 - roi_x1, roi_dy = roi_y2 - roi_y1;
    const int picturesize = roi_dx*roi_dy;
    void*_data=static_cast<void*>(img.data);
    T*data = static_cast<T*>(_data);
    T*buffer = data;
    int counter, n;
    int i = 0;

    if (doROI) {
      buffer = data + csize*(roi_y1 * xsize + roi_x1);
    }

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
        i++;
        if (doROI) {
          buffer = data + csize*(( i / roi_dx + roi_y1 ) * xsize + (i % roi_dx) + roi_x1);
        } else {
          buffer+=4;
        }
      }
      break;
    case GEM_GRAY:
      counter=(picturesize<argc)?picturesize:argc;
      if (csize == 4) {
        while (counter--) {
          T value;
          if (std::is_same<T, unsigned char>::value) {
            value = static_cast<T>(inputScale*atom_getfloat(argv));   // rgb
          } else {
            value = static_cast<T>(atom_getfloat(argv));   // rgb
          }
          buffer[chRed] = buffer[chGreen] = buffer[chBlue] = value;
          buffer[chAlpha] = static_cast<T>(0);                                    // alpha
          argv++;
          i++;
          if (doROI) {
            buffer = data + csize*(( i / roi_dx + roi_y1 ) * xsize + (i % roi_dx) + roi_x1);
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
          i++;
          if (doROI) {
            buffer = data + csize*(( i / roi_dx + roi_y1 ) * xsize + (i % roi_dx) + roi_x1);
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
        i++;
        if (doROI) {
          buffer = data + csize*(( i / roi_dx + roi_y1 ) * xsize + (i % roi_dx) + roi_x1);
        } else {
          buffer+=4;
        }
      }
    }
  }

  template<typename T>
  void fillPixelData(imageStruct&img, int argc, t_atom *argv,
                     int mode, float inputScale,
                     int roi_x1, int roi_x2, int roi_y1, int roi_y2, bool doROI) {
    const int csize = img.csize, xsize = img.xsize;
    const int roi_dx = roi_x2 - roi_x1, roi_dy = roi_y2 - roi_y1;
    const int picturesize = roi_dx*roi_dy;
    void*_data=static_cast<void*>(img.data);
    T*data = static_cast<T*>(_data);
    T*buffer = data;

    int counter = picturesize, i=0;
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
        i++;
        if (doROI) {
          buffer = data + 4*(( i / roi_dx + roi_y1 ) * xsize + (i % roi_dx) + roi_x1);
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
        i++;
        if (doROI) {
          buffer = data + 4*(( i / roi_dx + roi_y1 ) * xsize + (i % roi_dx) + roi_x1);
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
        i++;
        if (doROI) {
          buffer = data + 4*(( i / roi_dx + roi_y1 ) * xsize + (i % roi_dx) + roi_x1) ;
        } else {
          buffer+=4;
        }
      }
    }
  }

  bool setFormat(imageStruct&img, int reqformat, int reqtype) {
    bool changed = false;
    if(GEM_RGB == reqformat)
      reqformat = GEM_RGBA;
    if(img.format != reqformat)
      changed = True;
    img.setFormat(reqformat);

    if(img.type != reqtype)
      changed = True;
    img.type = reqtype;

    void*data = img.data;
    img.reallocate();
    if(data != img.data)
      changed =True;

    img.upsidedown = true; // Reset upsidedown for consistency

    return changed;
  }
};

/////////////////////////////////////////////////////////
// DATAMess
//
/////////////////////////////////////////////////////////
void pix_set :: DATAMess(t_symbol* s, int argc, t_atom *argv)
{
  int i = 0;
  bool setblack = false;

  if(setFormat(m_pixBlock.image, m_mode, m_reqType) && !m_pixels)
    setblack = True;

  pixBlock*pixels=m_pixels?m_pixels:&m_pixBlock;
  auto &img = pixels->image;

  int roi_x1=0;
  int roi_x2=img.xsize;
  int roi_y1=0;
  int roi_y2=img.ysize;

  if (!m_doROI) {
    // if no ROI is set, set whole image black before setting pixels values
    setblack = true;
  } else {
    roi_x1=m_roi.x1*(0.5+img.xsize);
    roi_x2=m_roi.x2*(0.5+img.xsize);
    roi_y1=m_roi.y1*(0.5+img.ysize);
    roi_y2=m_roi.y2*(0.5+img.ysize);
  }

  if(setblack)
    img.setBlack();

  // Handle different data types using template functions
  switch(img.type) {
  case GL_UNSIGNED_BYTE: // BYTE mode
    setPixelData<unsigned char>(img, argc, argv,
                                 m_mode, m_inputScale,
                                 roi_x1, roi_x2, roi_y1, roi_y2, m_doROI);
    break;
  case GL_FLOAT:
    setPixelData<GLfloat>(img, argc, argv,
                           m_mode, 1.0,
                           roi_x1, roi_x2, roi_y1, roi_y2, m_doROI);
    break;
  case GL_DOUBLE:
    setPixelData<GLdouble>(img, argc, argv,
                            m_mode, 1.0,
                            roi_x1, roi_x2, roi_y1, roi_y2, m_doROI);
    break;
  default:
    error("unknown data type 0x%04X", img.type);
    return;
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

void pix_set :: csMess(std::string cs)
{
  std::string color;
  unsigned int fmt=GEM_RGBA;
  char c=0;
  int len = cs.size();
  if(len>0) {
    char col[5];
    int i;
    if(len > 4) {
      len = 4;
    }
    for(i=0; i<len; i++) {
      col[i] = tolower(cs[i]);
    }
    col[len] = 0;
    color = col;
  }
  if ("rgba" == color) {
    RGBAMess();
  } else if ("rgb" == color) {
    RGBMess();
#if 0
  } else if ("yuv" == color) {
    YUVMess();
#endif
  } else if (("grey" == color) || ("gray" == color)) {
    GREYMess();
  } else {
    error("invalid colorspace '%s'; must be 'rgba', 'yuv' or 'grey'",
          cs.c_str());
    return;
  }
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
// CLEARMess
//
/////////////////////////////////////////////////////////
void pix_set :: FILLMess(t_symbol* s, int argc, t_atom *argv)
{
  int i=0;
  void *buffer;

  setFormat(m_pixBlock.image, m_mode, m_reqType);
  pixBlock*pixels=m_pixels?m_pixels:&m_pixBlock;
  imageStruct&img = pixels->image;

  int roi_x1=0;
  int roi_x2=img.xsize;
  int roi_y1=0;
  int roi_y2=img.ysize;

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
    img.setBlack();
    buffer = img.data;
  } else {
    roi_x1=m_roi.x1*(0.5+img.xsize);
    roi_x2=m_roi.x2*(0.5+img.xsize);
    roi_y1=m_roi.y1*(0.5+img.ysize);
    roi_y2=m_roi.y2*(0.5+img.ysize);
    const int roi_dx = roi_x2 - roi_x1, roi_dy = roi_y2 - roi_y1;

    buffer = img.data + img.csize*(( i /
             roi_dx + roi_y1 ) * img.xsize + (i %
                 roi_dx) + roi_x1) ;
  }

  // Handle different data types using template functions
  switch(img.type) {
  case GL_UNSIGNED_BYTE: // BYTE mode
    fillPixelData<unsigned char>(img, argc, argv,
                                 m_mode, m_inputScale,
                                 roi_x1, roi_x2, roi_y1, roi_y2, m_doROI);
    break;
  case GL_FLOAT:
    fillPixelData<GLfloat>(img, argc, argv,
                           m_mode, 1.0,
                           roi_x1, roi_x2, roi_y1, roi_y2, m_doROI);
    break;
  case GL_DOUBLE:
    fillPixelData<GLdouble>(img, argc, argv,
                            m_mode, 1.0,
                            roi_x1, roi_x2, roi_y1, roi_y2, m_doROI);
    break;
  default:
    error("unknown data type 0x%04X", img.type);
    return;
  }
  pixels->newimage = true;
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
    return;
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
  CPPEXTERN_MSG1(classPtr, "colorspace", csMess, std::string);


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

  CPPEXTERN_MSG2(classPtr, "dimen", SETMess, int, int);
  CPPEXTERN_MSG2(classPtr, "set", SETMess, int, int);

  CPPEXTERN_MSG (classPtr, "fill", FILLMess);
  CPPEXTERN_MSG0(classPtr, "bang", BANGMess);
  CPPEXTERN_MSG1(classPtr, "bytemode", bytemodeMess, bool);
  CPPEXTERN_MSG1(classPtr, "type", typeMess, std::string);
}
