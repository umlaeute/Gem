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
  m_quality(NONE)
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

    unsigned char r[2][2], g[2][2], b[2][2], a[2][2], G[2][2];

    m_pixRight->image.getRGB(ixPos0, iyPos0, &r[0][0], &g[0][0], &b[0][0],
                             &a[0][0]);
    m_pixRight->image.getRGB(ixPos1, iyPos0, &r[1][0], &g[1][0], &b[1][0],
                             &a[1][0]);
    m_pixRight->image.getRGB(ixPos0, iyPos1, &r[0][1], &g[0][1], &b[0][1],
                             &a[0][1]);
    m_pixRight->image.getRGB(ixPos1, iyPos1, &r[1][1], &g[1][1], &b[1][1],
                             &a[1][1]);

    m_pixRight->image.getGrey(ixPos0, iyPos0, &G[0][0]);
    m_pixRight->image.getGrey(ixPos1, iyPos0, &G[1][0]);
    m_pixRight->image.getGrey(ixPos0, iyPos1, &G[0][1]);
    m_pixRight->image.getGrey(ixPos1, iyPos1, &G[1][1]);

    t_float xy00=(1-xFrac)*(1-yFrac);
    t_float xy01=(1-xFrac)*   yFrac ;
    t_float xy10=   xFrac *(1-yFrac);
    t_float xy11=   xFrac *   yFrac ;

#define INTERPOLATE_LIN2D(x) (xy00*x[0][0] + xy01*x[0][1] + xy10*x[1][0] + xy11*x[1][1])
    red   = INTERPOLATE_LIN2D(r) / 255.;
    green = INTERPOLATE_LIN2D(g) / 255.;
    blue  = INTERPOLATE_LIN2D(b) / 255.;
    alpha = INTERPOLATE_LIN2D(a) / 255.;
    grey  = INTERPOLATE_LIN2D(G) / 255.;
  }
  break;
  case NONE: {
    unsigned char r, g, b, a, G;
    m_pixRight->image.getRGB(ixPos0, iyPos0, &r, &g, &b, &a);
    m_pixRight->image.getGrey(ixPos0, iyPos0, &G);

#define INTERPOLATE_NONE(x) (x)
    red   = INTERPOLATE_NONE(r) / 255.;
    green = INTERPOLATE_NONE(g) / 255.;
    blue  = INTERPOLATE_NONE(b) / 255.;
    alpha = INTERPOLATE_NONE(a) / 255.;
    grey  = INTERPOLATE_NONE(G) / 255.;
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
void pix_data :: listMess(t_symbol *s, int argc, t_atom* argv)
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
}
