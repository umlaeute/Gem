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
// tv_movement
//
//
/////////////////////////////////////////////////////////

#include "tv_movement.h"
#include <string.h>
#include <math.h>

CPPEXTERN_NEW(tv_movement)

  /////////////////////////////////////////////////////////
//
// tv_movement
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
tv_movement :: tv_movement()
{ 
  imageStruct image;

  image.xsize  = image.ysize = 64;
  image.format = GL_RGBA;
  image.csize  = 4;

  create_buffer(image);

  bufcount  = 0;
  treshold = 127;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
tv_movement :: ~tv_movement()
{
  // clean my buffer
  delete_buffer();
}

/////////////////////////////////////////////////////////
// CreateBuffer
//
/////////////////////////////////////////////////////////
void tv_movement :: create_buffer(imageStruct image)
{
  long dataSize;

  buffer.xsize = image.xsize;
  buffer.ysize = image.ysize;
  buffer.csize = 1;

  dataSize = buffer.xsize * buffer.ysize * buffer.csize * sizeof(unsigned char);

  buffer.data = new unsigned char[dataSize];
  memset(buffer.data, 0, dataSize);
}

/////////////////////////////////////////////////////////
// DeleteBuffer
//
/////////////////////////////////////////////////////////
void tv_movement :: delete_buffer()
{
  delete [] buffer.data;
  buffer.data = NULL;
}


/////////////////////////////////////////////////////////
// ClearBuffer
//
/////////////////////////////////////////////////////////
void tv_movement :: clear_buffer()
{
  memset(buffer.data, 0, buffer.xsize * buffer.ysize * buffer.csize * sizeof(unsigned char));
}


/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void tv_movement :: processImage(imageStruct &image)
{
  // assume that the pix_size does not change !
  if (image.xsize != buffer.xsize || image.ysize != buffer.ysize) {
    long dataSize;
    delete [] buffer.data;
    buffer.xsize = image.xsize;
    buffer.ysize = image.ysize;
    //		buffer.csize = image.csize;

    dataSize = buffer.xsize * buffer.ysize * buffer.csize * sizeof(unsigned char);
    buffer.data = new unsigned char[dataSize];

    memset(buffer.data, 0, dataSize);
  }

  int pixsize = image.ysize * image.xsize;

  unsigned char *rp = image.data;			// read pointer
  unsigned char *wp=buffer.data;			// write pointer

  while(pixsize--) {
    unsigned char grey = (unsigned char)(rp[chRed] * 0.3086f + rp[chGreen] * 0.6094f + rp[chBlue] * 0.0820f);
	
    rp[chAlpha] = 255*(fabs((unsigned char)grey-*wp)>treshold);
    *wp++=(unsigned char)grey;
    rp+=4;
  } 
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void tv_movement :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classtv_movement, gensym("pix_movement"), A_NULL);

  class_addmethod(classPtr, (t_method)&tv_movement::treshMessCallback,
		  gensym("tresh"), A_FLOAT, A_NULL);

}
void tv_movement :: treshMessCallback(void *data, t_floatarg newmode)
{
  GetMyClass(data)->treshold=(unsigned char)(255*newmode);
  post("treshold = %d", GetMyClass(data)->treshold);
}
