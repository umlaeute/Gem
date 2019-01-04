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
//    Copyright (c) 2012 Ricardo Fabbri. labmacambira.sf.net rfabbri@gmail.com
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_equal.h"

CPPEXTERN_NEW(pix_equal);

/////////////////////////////////////////////////////////
//
// pix_equal
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_equal :: pix_equal()
{
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"),
            gensym("vec_lower"));
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"),
            gensym("vec_upper"));
  m_upper[0] = m_upper[1] = m_upper[2] = m_upper[3] = 255;
  m_lower[0] = m_lower[1] = m_lower[2] = m_lower[3] = 0;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_equal :: ~pix_equal()
{ }

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_equal :: processRGBAImage(imageStruct &image)
{
  int datasize = image.xsize * image.ysize;

  unsigned char *base = image.data;

  while(datasize--) {
    unsigned char pixval =
      (base[chRed] <= m_upper[chRed] && base[chRed] >= m_lower[chRed] &&
       base[chGreen] <= m_upper[chGreen] && base[chGreen] >= m_lower[chGreen] &&
       base[chBlue] <= m_upper[chBlue] && base[chBlue] >= m_lower[chBlue])*255;

    base[chRed] = base[chGreen] = base[chBlue] = pixval;
    base[chAlpha] = 255;
    base += 4;
  }
}

namespace
{
static void setvec(const char*label, unsigned char*dest, int argc,
                   t_atom*argv)
{
  switch(argc) {
  case 4:
    dest[chAlpha]=atom_getfloat(argv+3)*255.;
  case 3:
    dest[chRed  ]=atom_getfloat(argv+0)*255.;
    dest[chGreen]=atom_getfloat(argv+1)*255.;
    dest[chBlue ]=atom_getfloat(argv+2)*255.;
    break;
  case 1:
    dest[chRed]=dest[chGreen]=dest[chBlue]=atom_getfloat(argv+0)*255.;
    break;
  default:
    error("illegal number of arguments for %s, must be 1, 3 or 4", label);
    break;
  }
}
};

void pix_equal :: vecUpperBoundMess(t_symbol*s,int argc, t_atom *argv)
{
  m_upper[chAlpha] = 255;
  setvec("upper bound", m_upper, argc, argv);

  setPixModified();
}

void pix_equal :: vecLowerBoundMess(t_symbol*s,int argc, t_atom *argv)
{
  m_lower[chAlpha] = 0;
  setvec("lower bound", m_lower, argc, argv);

  setPixModified();
}

void pix_equal :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG (classPtr, "vec_lower", vecLowerBoundMess);
  CPPEXTERN_MSG (classPtr, "vec_upper", vecUpperBoundMess);
}
