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
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
//
// pix_histo
//
//   IOhannes m zmoelnig
//   mailto:zmoelnig@iem.kug.ac.at
//
//   this code is published under the Gnu GeneralPublicLicense that should be distributed with gem & pd
//
/////////////////////////////////////////////////////////

#include "pix_histo.h"
#include <string.h>
#include <math.h>

CPPEXTERN_NEW_WITH_GIMME(pix_histo)


  /////////////////////////////////////////////////////////
//
// pix_histo
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_histo :: pix_histo(int argc, t_atom *argv)
{ 
  setMess(argc, argv);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_histo :: ~pix_histo()
{
}


////////////////////////////////////
// Set Message
//
///////////////////////////////////
void pix_histo :: setMess(int argc, t_atom *argv)
{
  t_atom *ap=argv;
  int n=argc;

  if (!(argc==1 || argc==3 || argc==4)) {
    error("pix_histo: only 1, 3 or 4 arguments are allowed");
    m_mode=0;
    return;
  }

  while(n--){
    if (ap->a_type != A_SYMBOL) {
      error("pix_histo: only symbolic table-names are accepted");
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
}


///////////////
// check if array exists and whether it is a floatarray
//
///////////////
t_float* pix_histo :: checkarray(t_symbol *s, int *length)
{
    t_garray *a;
    t_float  *fp;
    *length = 0;

    if (!(a = (t_garray *)pd_findbyclass(s, garray_class)))
    {
    	if (*s->s_name) error("pix_histo: %s: no such array", s->s_name);
    	fp = 0;
    }
    else if (!garray_getfloatarray(a, length, &fp))
    {
    	error("%s: bad template for tabwrite~", s->s_name);
    	fp = 0;
    }

    if (*length==0){
      error("pix_histo: table %s is zero-lengthed", s->s_name);
      fp=0;
    }
    return fp;
}

///////////////
// update graphs
//
///////////////
void pix_histo :: update_graphs(void)
{
  t_garray *a;

  switch (m_mode) {
  case 4:
    if ((a = (t_garray *)pd_findbyclass(name_A, garray_class)))
      garray_redraw(a);
  case 3:
    if ((a = (t_garray *)pd_findbyclass(name_G, garray_class)))
      garray_redraw(a);
    if ((a = (t_garray *)pd_findbyclass(name_B, garray_class)))
      garray_redraw(a);
  case 1:
    if ((a = (t_garray *)pd_findbyclass(name_R, garray_class)))
      garray_redraw(a);
  default:
    break;
  }
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_histo :: processRGBAImage(imageStruct &image)
{
  int i=image.xsize*image.ysize;
  unsigned char *base = image.data;
  
  int n_R, n_G, n_B, n_A;
  t_float *tab_R=NULL, *tab_G=NULL, *tab_B=NULL, *tab_A=NULL;
  t_float scale_R=0, scale_G=0, scale_B=0, scale_A=0;

  t_float f;

  int n;
  t_float *tab;

  if (m_mode==0) return;
  switch (m_mode) {
  case 4:
    if (!(tab_A=checkarray(name_A, &n_A))) return;
    scale_A=n_A/256.;
    n = n_A;    tab = tab_A;    while(n--)*tab++=0;
  case 3:
    if (!(tab_G=checkarray(name_G, &n_G))) return;
    scale_G=n_G/256.;
    n = n_G;    tab = tab_G;    while(n--)*tab++=0;
    if (!(tab_B=checkarray(name_B, &n_B))) return;
    scale_B=n_B/256.;
    n = n_B;    tab = tab_B;    while(n--)*tab++=0;
  case 1:
    if (!(tab_R=checkarray(name_R, &n_R))) return;
    scale_R=n_R/256.;
    n = n_R;    tab = tab_R;    while(n--)*tab++=0;
  default:
    break;
  }

  f = 1./i;

  switch (m_mode) {
  case 1: // RGB->grey
    while (i--) {
      float grey = base[chRed] * 0.3086f + base[chGreen] * 0.6094f
	+ base[chBlue] * 0.0820f;

      //      *(tab_R+(int)(scale_R*(unsigned char)grey))
      *(tab_R+(int)(scale_R*grey))+=f;

      base+=4;
    }
    break;
  case 3: // RGB
    while (i--) {
      *(tab_R+(int)(scale_R*base[chRed]))  +=f;
      *(tab_G+(int)(scale_G*base[chGreen]))+=f;
      *(tab_B+(int)(scale_B*base[chBlue])) +=f;

      base+=4;
    }
    break;
  case 4: // RGBA
    while (i--) {
      *(tab_R+(int)(scale_R*base[chRed]))  +=f;
      *(tab_G+(int)(scale_G*base[chGreen]))+=f;
      *(tab_B+(int)(scale_B*base[chBlue])) +=f;
      *(tab_A+(int)(scale_B*base[chAlpha]))+=f;

      base+=4;
    }
  default:
    break;
  }

  update_graphs();
}

void pix_histo :: processYUVImage(imageStruct &image)
{
  int i=image.xsize*image.ysize;
  unsigned char *base = image.data;
  
  int n_Y, n_U, n_V;
  t_float *tab_Y=NULL, *tab_U=NULL, *tab_V=NULL;
  int n;
  t_float *tab;

  if (m_mode==0) return;
  switch (m_mode) {
  case 3:
    if (!(tab_V=checkarray(name_B, &n_V))) return;
    n = n_V;    tab = tab_V;    while(n--)*tab++=0;
    if (!(tab_U=checkarray(name_G, &n_U))) return;
    n = n_U;    tab = tab_U;    while(n--)*tab++=0;
  case 1:
    if (!(tab_Y=checkarray(name_R, &n_Y))) return;
    n = n_Y;    tab = tab_Y;    while(n--)*tab++=0;
  default:
    break;
  }

  t_float f = 1./i;
  t_float f2 = f*2.f;

  i/=2;
  switch (m_mode) {
  case 1: // RGB->grey
    while (i--) {
      *(tab_Y+((n_Y*base[chY0])>>8))+=f;
      *(tab_Y+((n_Y*base[chY1])>>8))+=f;
      base+=4;
    }
    break;
  case 3: // RGB
    while (i--) {
      *(tab_Y+((n_Y*base[chY0])>>8))  +=f;
      *(tab_Y+((n_Y*base[chY1])>>8))  +=f;
      *(tab_U+((n_U*base[chU])>>8))   +=f2;
      *(tab_V+((n_V*base[chV])>>8))   +=f2;
      base+=4;
    }
    break;
  default:
    break;
  }
  update_graphs();
}

void pix_histo :: processGrayImage(imageStruct &image)
{
  int i=image.xsize*image.ysize;
  unsigned char *base = image.data;
  
  int n_G;
  t_float *tab_G=NULL;
  t_float scale_R=0;
  t_float f;

  int n;
  t_float *tab;

  if (m_mode==0) return;
  
  if (!(tab_G=checkarray(name_A, &n_G))) return;
  n = n_G;    tab = tab_G;    while(n--)*tab++=0;

  f = 1./i;

  while (i--)*(tab_G+((n_G*(*base++))>>8))  +=f;

  update_graphs();
}
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_histo :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_histo::setMessCallback,
		  gensym("set"), A_GIMME,0);
}

void pix_histo :: setMessCallback(void *data, t_symbol *s, int argc, t_atom* argv)
{
    GetMyClass(data)->setMess(argc, argv);
}
