////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
//
// pix_curve
//
//   IOhannes m zmoelnig
//   mailto:zmoelnig@iem.kug.ac.at
//
//   this code is published under the Gnu GeneralPublicLicense that should be distributed with gem & pd
//
/////////////////////////////////////////////////////////

#include "pix_curve.h"
#include <string.h>
#include <math.h>

CPPEXTERN_NEW_WITH_GIMME(pix_curve)


  /////////////////////////////////////////////////////////
//
// pix_curve
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_curve :: pix_curve(int argc, t_atom *argv)
{ 
  setMess(argc, argv);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_curve :: ~pix_curve()
{
}


////////////////////////////////////
// Set Message
//
///////////////////////////////////
void pix_curve :: setMess(int argc, t_atom *argv)
{
  t_atom *ap=argv;
  int n=argc;

  if (!(argc==1 || argc==3 || argc==4)) {
    error("only 1, 3 or 4 arguments are allowed");
    m_mode=0;
    return;
  }

  while(n--){
    if (ap->a_type != A_SYMBOL) {
      error("only symbolic table-names are accepted");
      return;
    }
    ap++;
  }

  m_mode=3;

  ap=argv;
  switch (argc) {
  case 1:
    name_R=name_G=name_B=name_A=atom_getsymbol(ap);
    m_mode=1;
    break;
  case 4:
    name_A=atom_getsymbol(ap+3);
    m_mode=4;
  default:
    name_R=atom_getsymbol(ap);
    name_G=atom_getsymbol(ap+1);
    name_B=atom_getsymbol(ap+2);
  }
  setPixModified();
}


///////////////
// check if array exists and whether it is a floatarray
//
///////////////
t_float* pix_curve :: checkarray(t_symbol *s, int *length)
{
    t_garray *a;
    t_float  *fp;
    *length = 0;

    if (!(a = (t_garray *)pd_findbyclass(s, garray_class)))
    {
    	if (*s->s_name) error("%s: no such array", s->s_name);
    	fp = 0;
    }
    else if (!garray_getfloatarray(a, length, &fp))
    {
    	error("%s: bad template for tabwrite~", s->s_name);
    	fp = 0;
    }

    if (*length==0){
      error("table %s is zero-lengthed", s->s_name);
      fp=0;
    }
    return fp;
}


/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_curve :: processRGBAImage(imageStruct &image)
{
  int i=image.xsize*image.ysize;
  unsigned char *base = image.data;
  
  int n_R, n_G, n_B, n_A;
  t_float *tab_R, *tab_G, *tab_B, *tab_A;

  int r, g, b, a;

  if (m_mode==0) return;

  if (!(tab_R=checkarray(name_R, &n_R))) return;
  if (!(tab_G=checkarray(name_G, &n_G))) return;
  if (!(tab_B=checkarray(name_B, &n_B))) return;

  switch (m_mode) {
  case 3: // only RGB
    while (i--) {
      r = static_cast<int>(*(tab_R+((n_R*base[chRed  ])>>8)));
      g = static_cast<int>(*(tab_G+((n_G*base[chGreen])>>8)));
      b = static_cast<int>(*(tab_B+((n_B*base[chBlue ])>>8)));
   
      base[chRed]   = CLAMP(r);
      base[chGreen] = CLAMP(g);
      base[chBlue]  = CLAMP(b);

      base+=4;
    }
    break;
  case 4: // RGBA
  case 1: // one table for all
    if (!(tab_A=checkarray(name_A, &n_A))) return;
    
    while (i--) {
      r = static_cast<int>(*(tab_R+((n_R*base[chRed])>>8)));
      g = static_cast<int>(*(tab_G+((n_G*base[chGreen])>>8)));
      b = static_cast<int>(*(tab_B+((n_B*base[chBlue])>>8)));
      a = static_cast<int>(*(tab_A+((n_A*base[chAlpha])>>8)));
   
      base[chRed]   = CLAMP(r);
      base[chGreen] = CLAMP(g);
      base[chBlue]  = CLAMP(b);
      base[chAlpha] = CLAMP(a);

      base+=4;
    }
  default:
    break;
  }


}
/////////////////////////////////////////////////////////
// processImage
void pix_curve :: processGrayImage(imageStruct &image)
{
  int i=image.xsize*image.ysize;
  unsigned char *base = image.data;
  
  int n;
  t_float *tab;
  int val;

  if (!(tab=checkarray(name_R, &n))) return;
  while (i--) {
    val = static_cast<int>(*(tab+static_cast<int>((n*base[chGray])>>8)));
    *base++   = CLAMP(val);
  }
}

void pix_curve :: processYUVImage(imageStruct &image)
{
  int i=image.xsize*image.ysize/2;
  unsigned char *base = image.data;
  
  int n_Y, n_U, n_V;
  t_float *tab_Y, *tab_U, *tab_V;

  int y0, y1, u, v;

  if (m_mode==0) return;

  if (!(tab_Y=checkarray(name_R, &n_Y))) return;
  if (!(tab_U=checkarray(name_G, &n_U))) return;
  if (!(tab_V=checkarray(name_B, &n_V))) return;
  switch (m_mode) {
  case 3: // YUV
    while (i--) {
      u  = static_cast<int>(*(tab_U+((n_U*base[chU])>>8)));
      y0 = static_cast<int>(*(tab_Y+((n_Y*base[chY0])>>8)));
      v  = static_cast<int>(*(tab_V+((n_V*base[chV])>>8)));
      y1 = static_cast<int>(*(tab_Y+((n_Y*base[chY1])>>8)));

      base[chU]  = CLAMP(u);
      base[chY0] = CLAMP(y0);
      base[chV]  = CLAMP(v);
      base[chY1] = CLAMP(y1);

      base+=4;
    }
    break;
  case 1: // only Y
    if (!(tab_Y=checkarray(name_A, &n_Y))) return;
    
    while (i--) {
      y0 = static_cast<int>(*(tab_Y+((n_Y*base[chY0])>>8)));
      y1 = static_cast<int>(*(tab_Y+((n_Y*base[chY1])>>8)));
   
      base[chY0] = CLAMP(y0);
      base[chY1] = CLAMP(y1);

      base+=4;
    }
  default:
    break;
  }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_curve :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_curve::setMessCallback),
		  gensym("set"), A_GIMME,0);
}

void pix_curve :: setMessCallback(void *data, t_symbol *s, int argc, t_atom* argv)
{
    GetMyClass(data)->setMess(argc, argv);
}
