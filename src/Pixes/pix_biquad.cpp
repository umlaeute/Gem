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
// pix_biquad
//
//   IOhannes m zmoelnig
//   mailto:zmoelnig@iem.kug.ac.at
//
//   this code is published under the Gnu GeneralPublicLicense that should be distributed with gem & pd
//
/////////////////////////////////////////////////////////

#include "pix_biquad.h"
#include <string.h>
#include <math.h>

CPPEXTERN_NEW_WITH_GIMME(pix_biquad)

  /////////////////////////////////////////////////////////
//
// pix_biquad
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_biquad :: pix_biquad(int argc, t_atom*argv) :
  set(false), fb0(1), fb1(0), fb2(0), ff1(1), ff2(0), ff3(0), m_mode(0)
{ 
  prev.xsize = 64;
  prev.ysize = 64;
  prev.setCsizeByFormat(GL_RGBA);
  prev.reallocate();
  prev.setBlack();

  last.xsize = 64;
  last.ysize = 64;
  last.setCsizeByFormat(GL_RGBA);
  last.csize = 4;
  last.reallocate();
  last.setBlack();

  if(argc)faktorMess(argc, argv);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_biquad :: ~pix_biquad()
{
  // clean my buffer
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_biquad :: processImage(imageStruct &image)
{
  // assume that the pix_size does not change !
  bool do_blank=(image.xsize!=prev.xsize || image.ysize!=prev.ysize || image.csize!=prev.csize);
  prev.xsize = image.xsize;
  prev.ysize = image.ysize;
  prev.csize = image.csize;
  prev.reallocate();
  last.xsize = image.xsize;
  last.ysize = image.ysize;
  last.csize = image.csize;
  last.reallocate();

  if (set) { 
    memcpy(prev.data, image.data, image.ysize * image.xsize * image.csize);
    memcpy(last.data, image.data, image.ysize * image.xsize * image.csize);
    set = false;
  } else if (do_blank){
    prev.setBlack();
    last.setBlack();
  }

  int pixsize = image.ysize * image.xsize * image.csize;

  unsigned char *this_p = image.data;
  unsigned char *last_p= last.data;
  unsigned char *prev_p= prev.data;

  //  post("%f %f %f\t%f %f %f", fb0, fb1, fb2, ff1, ff2, ff3);

  if (m_mode){
    // slow, because calculations are done in float !
    while(pixsize--) {
      float output;
      output = fb0 * *this_p + fb1 * *last_p + fb2 * *prev_p;
      *this_p++    = (unsigned char)(ff1 * output + ff2 * *last_p + ff3 * *prev_p);
      *prev_p++	 = *last_p;
      *last_p++	 = (unsigned char)output;
    }
  }else{
    // fast, because calculations are done in int !
    int ifb0,ifb1,ifb2,iff1,iff2,iff3;
    int ioutput;
    ifb0 = (int)(256. * fb0);
    ifb1 = (int)(256. * fb1);
    ifb2 = (int)(256. * fb2);
    iff1 = (int)(256. * ff1);
    iff2 = (int)(256. * ff2);
    iff3 = (int)(256. * ff3);
    
    while(pixsize--) {
      ioutput = ((ifb0 * *this_p) + (ifb1 * *last_p) + (ifb2 * *prev_p)>>8);
      *this_p++    = (unsigned char)CLAMP((iff1 * ioutput) + (iff2 * *last_p) + (iff3 * *prev_p)>>8);
      *prev_p++	 = *last_p;
      *last_p++	 = (unsigned char)CLAMP(ioutput);
    }
  }
}

void pix_biquad :: faktorMess(int argc, t_atom*argv){
  if (argc<5 || argc>6){
    error("pix_biquad: illegal number of arguments");
    return;
  }

  if (argc==6)fb0=atom_getfloat(argv++);
  else fb0=1;

  fb1=atom_getfloat(argv++);
  fb2=atom_getfloat(argv++);
  ff1=atom_getfloat(argv++);
  ff2=atom_getfloat(argv++);
  ff3=atom_getfloat(argv++);
}

///////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_biquad :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_biquad::setMessCallback,
		  gensym("set"), A_NULL);
                  
  class_addmethod(classPtr, (t_method)&pix_biquad::modeMessCallback,
		  gensym("mode"), A_DEFFLOAT, A_NULL);
  class_addlist(classPtr, (t_method)&pix_biquad::faktorMessCallback);
}

void pix_biquad :: faktorMessCallback(void *data, t_symbol *s, int argc, t_atom* argv)
{
  GetMyClass(data)->faktorMess(argc, argv);
}

void pix_biquad :: setMessCallback(void *data)
{
  GetMyClass(data)->set = true;
}

void pix_biquad :: modeMessCallback(void *data, float value)
{
  GetMyClass(data)->m_mode = (int)value;
}
