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

CPPEXTERN_NEW(pix_biquad)

  /////////////////////////////////////////////////////////
//
// pix_biquad
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_biquad :: pix_biquad()
{ 
  imageStruct image;

  image.xsize  = image.ysize = 64;
  image.format = GL_RGBA;
  image.csize  = 4;

  create_buffer(image);

  fb0=fb1=fb2=ff1=ff2=ff3 = 0;

    m_mode = 1;
  set = false;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_biquad :: ~pix_biquad()
{
  // clean my buffer
  delete_buffer();
}

/////////////////////////////////////////////////////////
// CreateBuffer
//
/////////////////////////////////////////////////////////
void pix_biquad :: create_buffer(imageStruct image)
{
  long dataSize;

  prev.xsize = image.xsize;
  prev.ysize = image.ysize;
  prev.csize = image.csize;
  last.xsize = image.xsize;
  last.ysize = image.ysize;
  last.csize = image.csize;
  dataSize = prev.xsize * prev.ysize * prev.csize * sizeof(unsigned char);
 
  prev.data = new unsigned char[dataSize];
  memset(prev.data, 0, dataSize);
  last.data = new unsigned char[dataSize];
  memset(last.data, 0, dataSize);
}

/////////////////////////////////////////////////////////
// DeleteBuffer
//
/////////////////////////////////////////////////////////
void pix_biquad :: delete_buffer()
{
  delete [] prev.data;
  delete [] last.data;
  prev.data = NULL;
  last.data = NULL;
}


/////////////////////////////////////////////////////////
// ClearBuffer
//
/////////////////////////////////////////////////////////
void pix_biquad :: clear_buffer()
{
  memset(last.data, 0, last.xsize * last.ysize * last.csize * sizeof(unsigned char));
  memset(prev.data, 0, prev.xsize * prev.ysize * prev.csize * sizeof(unsigned char));
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_biquad :: processImage(imageStruct &image)
{
  // assume that the pix_size does not change !
  if (prev.xsize != image.xsize || prev.ysize != image.ysize) {
    long dataSize;
    delete [] prev.data;
    delete [] last.data;

    prev.xsize = image.xsize;
    prev.ysize = image.ysize;
    prev.csize = image.csize;
    last.xsize = image.xsize;
    last.ysize = image.ysize;
    last.csize = image.csize;

    dataSize = prev.xsize * prev.ysize * prev.csize * sizeof(unsigned char);
    prev.data = new unsigned char[dataSize];
    memset(prev.data, 0, dataSize);
    last.data = new unsigned char[dataSize];
    memset(last.data, 0, dataSize);
  }

  if (set) { 
    memcpy(prev.data, image.data, image.ysize * image.xsize * image.csize);
    memcpy(last.data, image.data, image.ysize * image.xsize * image.csize);
    set = false;
  } 

  int pixsize = image.ysize * image.xsize * image.csize;

  unsigned char *this_p = image.data;
  unsigned char *last_p= last.data;
  unsigned char *prev_p= prev.data;

  if (m_mode){

    while(pixsize--) {
        float output;
        output = fb0 * *this_p + fb1 * *last_p + fb2 * *prev_p;
        *this_p++    = (unsigned char)(ff1 * output + ff2 * *last_p + ff3 * *prev_p);
        *prev_p++	 = *last_p;
        *last_p++	 = (unsigned char)output;
    }
  }else{
    int ifb0,ifb1,ifb2,iff1,iff2,iff3;
    int ioutput;
    ifb0 = (int)(256. * fb0);
    ifb1 = (int)(256. * fb1);
    ifb2 = (int)(256. * fb2);
    iff1 = (int)(256. * ff1);
    iff2 = (int)(256. * ff2);
    iff3 = (int)( 256. * ff3);
    
    while(pixsize--) {
        
        //ioutput = ((ifb0 * *this_p)>>8 + (ifb1 * *last_p)>>8 + (ifb2 * *prev_p)>>8);
        //*this_p++    = (unsigned char)((iff1 * ioutput)>>8 + (iff2 * *last_p)>>8 + (iff3 * *prev_p)>>8);
        ioutput = ((ifb0 * *this_p) + (ifb1 * *last_p) + (ifb2 * *prev_p)>>8);
        *this_p++    = (unsigned char)CLAMP((iff1 * ioutput) + (iff2 * *last_p) + (iff3 * *prev_p)>>8);
        *prev_p++	 = *last_p;
        *last_p++	 = (unsigned char)CLAMP(ioutput);
    }

  }
 
}

/////////////////////////////////////////////////////////
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
  if (argc<5 || argc>6){
    GetMyClass(data)->fb0=GetMyClass(data)->fb1=GetMyClass(data)->fb2=0;
    GetMyClass(data)->ff1=GetMyClass(data)->ff2=GetMyClass(data)->ff3 = 0;
    error("pix_biquad: illegal number of arguments");
    return;
  }

  if (argc==6)GetMyClass(data)->fb0=atom_getfloat(argv++);
  else GetMyClass(data)->fb0=1;

  GetMyClass(data)->fb1=atom_getfloat(argv++);
  GetMyClass(data)->fb2=atom_getfloat(argv++);
  GetMyClass(data)->ff1=atom_getfloat(argv++);
  GetMyClass(data)->ff2=atom_getfloat(argv++);
  GetMyClass(data)->ff3=atom_getfloat(argv++);
}

void pix_biquad :: setMessCallback(void *data)
{
  GetMyClass(data)->set = true;
}

void pix_biquad :: modeMessCallback(void *data, float value)
{
  GetMyClass(data)->m_mode = (int)value;
}
