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
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_data.h"
#include "Utils/Functions.h"
#include "Gem/GemGL.h"

CPPEXTERN_NEW_WITH_TWO_ARGS(pix_data, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// pix_data
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_data :: pix_data(t_floatarg x, t_floatarg y) :
  m_quality(NONE), m_normalize(NORMALIZED)
{
  // create the new inlet for the X position
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
            gensym("xPos"));
  // create the new inlet for the Y position
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
            gensym("yPos"));

  m_colorOut = outlet_new(this->x_obj, 0);
  m_grayOut = outlet_new(this->x_obj, 0);

  m_position[0] = x;
  m_position[1] = y;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_data :: ~pix_data()
{
  outlet_free(m_colorOut);
  outlet_free(m_grayOut);
}

/////////////////////////////////////////////////////////
// Template functions for reading different data types
//
/////////////////////////////////////////////////////////
namespace {
  void getPixelData(imageStruct& image, int x, int y, t_float* r, t_float* g, t_float* b, t_float* a, t_float* G) {
    switch(image.type) {
    case GL_FLOAT:
      {
        GLfloat* data = reinterpret_cast<GLfloat*>(image.data);
        int offset = ((image.ysize - 1 - y) * image.xsize + x) * image.csize;
        if(image.csize == 1) {
          // Grey format
          *G = static_cast<t_float>(data[offset]);
          *r = *g = *b = *G;
          *a = 1.0f;
        } else if(image.csize == 3) {
          // RGB format (no alpha)
          *r = static_cast<t_float>(data[offset + chRed]);
          *g = static_cast<t_float>(data[offset + chGreen]);
          *b = static_cast<t_float>(data[offset + chBlue]);
          *a = 1.0f;
          // Use weighted average for grey calculation (same as Image.cpp)
          *G = static_cast<t_float>((*r * 77.0f + *g * 150.0f + *b * 29.0f) / 256.0f);
        } else {
          // RGBA format
          *r = static_cast<t_float>(data[offset + chRed]);
          *g = static_cast<t_float>(data[offset + chGreen]);
          *b = static_cast<t_float>(data[offset + chBlue]);
          *a = static_cast<t_float>(data[offset + chAlpha]);
          // Use weighted average for grey calculation (same as Image.cpp)
          *G = static_cast<t_float>((*r * 77.0f + *g * 150.0f + *b * 29.0f) / 256.0f);
        }
      }
      break;
    case GL_DOUBLE:
      {
        GLdouble* data = reinterpret_cast<GLdouble*>(image.data);
        int offset = ((image.ysize - 1 - y) * image.xsize + x) * image.csize;
        if(image.csize == 1) {
          // Grey format
          *G = static_cast<t_float>(data[offset]);
          *r = *g = *b = *G;
          *a = 1.0;
        } else if(image.csize == 3) {
          // RGB format (no alpha)
          *r = static_cast<t_float>(data[offset + chRed]);
          *g = static_cast<t_float>(data[offset + chGreen]);
          *b = static_cast<t_float>(data[offset + chBlue]);
          *a = 1.0;
          // Use weighted average for grey calculation (same as Image.cpp)
          *G = static_cast<t_float>((*r * 77.0 + *g * 150.0 + *b * 29.0) / 256.0);
        } else {
          // RGBA format
          *r = static_cast<t_float>(data[offset + chRed]);
          *g = static_cast<t_float>(data[offset + chGreen]);
          *b = static_cast<t_float>(data[offset + chBlue]);
          *a = static_cast<t_float>(data[offset + chAlpha]);
          // Use weighted average for grey calculation (same as Image.cpp)
          *G = static_cast<t_float>((*r * 77.0 + *g * 150.0 + *b * 29.0) / 256.0);
        }
      }
      break;
    default:
      {
        unsigned char ur, ug, ub, ua, uG;
        image.getRGB(x, y, &ur, &ug, &ub, &ua);
        image.getGrey(x, y, &uG);
        *r = static_cast<t_float>(ur / 255.0f);
        *g = static_cast<t_float>(ug / 255.0f);
        *b = static_cast<t_float>(ub / 255.0f);
        *a = static_cast<t_float>(ua / 255.0f);
        *G = static_cast<t_float>(uG / 255.0f);
      }
      break;
    }
  }
};

/////////////////////////////////////////////////////////
// trigger
//
/////////////////////////////////////////////////////////
void pix_data :: trigger()
{
  // if we don't have a "right hand" image, then just return
  if (!m_pixRight || !m_pixRight->image.data) {
    return;
  }

  t_float red, green, blue, alpha, grey;


  t_float maxX= m_pixRight->image.xsize - 1;
  t_float maxY= m_pixRight->image.ysize - 1;

  t_float fxPos = m_position[0];
  t_float fyPos = m_position[1];


  if(NORMALIZED == m_normalize) {
    fxPos *= (maxX+1);
    fyPos *= (maxY+1);
  }

  if(!m_pixRight->image.upsidedown) {
    fyPos = maxY - fyPos;
  }

  if(fxPos<0) {
    fxPos=0;
  }
  if(fxPos>=maxX) {
    fxPos=maxX;
  }
  if(fyPos<0) {
    fyPos=0;
  }
  if(fyPos>=maxY) {
    fyPos=maxY;
  }

  int ixPos0 = 0+static_cast<int>(fxPos);
  int iyPos0 = 0+static_cast<int>(fyPos);

  switch(m_quality) {
  default:
  case LINEAR2D: {
    int ixPos1 = 1+static_cast<int>(fxPos);
    int iyPos1 = 1+static_cast<int>(fyPos);

    if(ixPos1>=maxX) {
      ixPos1=ixPos0;
    }
    if(iyPos1>=maxY) {
      iyPos1=iyPos0;
    }

    t_float xFrac = fxPos - ixPos0;
    t_float yFrac = fyPos - iyPos0;

    t_float r[2][2], g[2][2], b[2][2], a[2][2], G[2][2];

    getPixelData(m_pixRight->image, ixPos0, iyPos0, &r[0][0], &g[0][0], &b[0][0], &a[0][0], &G[0][0]);
    getPixelData(m_pixRight->image, ixPos1, iyPos0, &r[1][0], &g[1][0], &b[1][0], &a[1][0], &G[1][0]);
    getPixelData(m_pixRight->image, ixPos0, iyPos1, &r[0][1], &g[0][1], &b[0][1], &a[0][1], &G[0][1]);
    getPixelData(m_pixRight->image, ixPos1, iyPos1, &r[1][1], &g[1][1], &b[1][1], &a[1][1], &G[1][1]);

    t_float xy00=(1-xFrac)*(1-yFrac);
    t_float xy01=(1-xFrac)*   yFrac ;
    t_float xy10=   xFrac *(1-yFrac);
    t_float xy11=   xFrac *   yFrac ;

#define INTERPOLATE_LIN2D(x) (xy00*x[0][0] + xy01*x[0][1] + xy10*x[1][0] + xy11*x[1][1])
    red   = INTERPOLATE_LIN2D(r);
    green = INTERPOLATE_LIN2D(g);
    blue  = INTERPOLATE_LIN2D(b);
    alpha = INTERPOLATE_LIN2D(a);
    grey  = INTERPOLATE_LIN2D(G);
  }
  break;
  case NONE: {
    t_float r, g, b, a, G;
    getPixelData(m_pixRight->image, ixPos0, iyPos0, &r, &g, &b, &a, &G);

#define INTERPOLATE_NONE(x) (x)
    red   = INTERPOLATE_NONE(r);
    green = INTERPOLATE_NONE(g);
    blue  = INTERPOLATE_NONE(b);
    alpha = INTERPOLATE_NONE(a);
    grey  = INTERPOLATE_NONE(G);
  }
  break;
  }

  t_atom atom[4];
  // send out the color information
  outlet_float(m_grayOut, grey);
  SETFLOAT(&atom[0], red);
  SETFLOAT(&atom[1], green);
  SETFLOAT(&atom[2], blue);
  SETFLOAT(&atom[3], alpha);
  outlet_list(m_colorOut, gensym("list"), 4, atom);
}

void pix_data :: xPos(t_float f)
{
  m_position[0]=f;
}
void pix_data :: yPos(t_float f)
{
  m_position[1]=f;
}
void pix_data :: listMess(t_symbol* s, int argc, t_atom* argv)
{
  switch(argc) {
  case 2:
    m_position[0] = atom_getfloat(argv+0);
    m_position[1] = atom_getfloat(argv+1);
    trigger();
    break;
  default:
    error("usage: list <xpos> <ypos>");
  }
}

void pix_data :: qualityMess(int q)
{
  switch((int)q) {
  case 0:
    m_quality = NONE;
    break;
  case 1:
    m_quality = LINEAR2D;
    break;
  default:
    error("quality must be 0|1");
  }
}
void pix_data :: normalizeMess(int q)
{
  switch((int)q) {
  case 0:
    m_normalize = RAW;
    break;
  case 1:
    m_normalize = NORMALIZED;
    break;
  default:
    error("normalize must be 0|1");
  }
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_data :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG0(classPtr, "bang", trigger);
  CPPEXTERN_MSG1(classPtr, "xPos", xPos, t_float);
  CPPEXTERN_MSG1(classPtr, "yPos", yPos, t_float);
  CPPEXTERN_MSG (classPtr, "list", listMess);

  CPPEXTERN_MSG1(classPtr, "quality", qualityMess, int);
  CPPEXTERN_MSG1(classPtr, "normalize", normalizeMess, int);
}
