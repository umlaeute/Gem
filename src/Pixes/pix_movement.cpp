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
// pix_movement
//
//
/////////////////////////////////////////////////////////

#include "pix_movement.h"
#include <string.h>
#include <math.h>

CPPEXTERN_NEW_WITH_ONE_ARG(pix_movement,t_floatarg, A_DEFFLOAT)

  /////////////////////////////////////////////////////////
//
// pix_movement
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_movement :: pix_movement(t_floatarg f)
{ 
  buffer.xsize  = buffer.ysize = 64;
  buffer.format = GL_LUMINANCE;
  buffer.csize  = 1;
  buffer.reallocate();
  buffer2.xsize  = buffer2.ysize = 64;
  buffer2.format = GL_LUMINANCE;
  buffer2.csize  = 1;
  buffer2.reallocate();


  if(f<=0.)f=0.5;
  if(f>1.f)f=1.0;
  treshold = (unsigned char)(255*f);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_movement :: ~pix_movement()
{
  // clean my buffer
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_movement :: processRGBAImage(imageStruct &image)
{
  // assume that the pix_size does not change !
  bool doclear=(image.xsize*image.ysize != buffer.xsize*buffer.ysize);
  buffer.xsize = image.xsize;
  buffer.ysize = image.ysize;
  buffer.reallocate();
  if(doclear) buffer.setWhite();

  int pixsize = image.ysize * image.xsize;

  unsigned char *rp = image.data;			// read pointer
  unsigned char *wp=buffer.data;			// write pointer

  while(pixsize--) {
    //    unsigned char grey = (unsigned char)(rp[chRed] * 0.3086f + rp[chGreen] * 0.6094f + rp[chBlue] * 0.0820f);
    //   rp[chAlpha] = 255*(fabs((unsigned char)grey-*wp)>treshold);
    unsigned char grey = (rp[chRed]*79+rp[chGreen]*156+rp[chBlue]*21)>>8;
    rp[chAlpha] = 255*(abs(grey-*wp)>treshold);
   *wp++=(unsigned char)grey;
    rp+=4;
  } 
}
void pix_movement :: processGrayImage(imageStruct &image)
{
  // assume that the pix_size does not change !
  bool doclear=(image.xsize*image.ysize != buffer.xsize*buffer.ysize);
  buffer.xsize = image.xsize;
  buffer.ysize = image.ysize;
  buffer.reallocate();
  if(doclear) buffer.setWhite();
  buffer2.xsize = image.xsize;
  buffer2.ysize = image.ysize;
  buffer2.reallocate();

  int pixsize = image.ysize * image.xsize;

  unsigned char *rp = image.data;			// read pointer
  unsigned char *wp=buffer.data;			// write pointer to the copy
  unsigned char *wp2=buffer2.data; // write pointer to the diff-image

  while(pixsize--) {
    unsigned char grey = *rp++;
    *wp2++=255*(abs(grey-*wp)>treshold);
    *wp++=grey;
  }
  image.data = buffer2.data;
  
}
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_movement :: obj_setupCallback(t_class *classPtr)
{

  class_addmethod(classPtr, (t_method)&pix_movement::treshMessCallback,
		  gensym("tresh"), A_FLOAT, A_NULL);

}
void pix_movement :: treshMessCallback(void *data, t_floatarg newmode)
{
  GetMyClass(data)->treshold=(unsigned char)(255*newmode);
  post("treshold = %d", GetMyClass(data)->treshold);
}
