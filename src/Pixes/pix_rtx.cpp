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
// pix_rtx
//
// this the RealTimeX - transformator, that does  a kind of swapping the time and the x axis
// of a video (or any series of pictures) ON THE FLY 
//
// for more information, on how this works, look at
// http://umlaeute.mur.at/rtx
//
//   2803:forum::für::umläute:2000
//   0409:forum::für::umläute:2000
//   1801:forum::für::umläute:2001  added the second mode
//   IOhannes m zmoelnig
//   mailto:zmoelnig@iem.kug.ac.at
//
//   this code is published under the Gnu GeneralPublicLicense that should be distributed with gem & pd
//
/////////////////////////////////////////////////////////

#include "pix_rtx.h"
#include <string.h>

CPPEXTERN_NEW(pix_rtx)

/////////////////////////////////////////////////////////
//
// pix_rtx
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_rtx :: pix_rtx()
{ 
   imageStruct image;

   image.xsize  = image.ysize = 64;
   image.format = GL_RGBA;
   image.csize  = 4;

   create_buffer(image);

   bufcount  = 0;
   mode = true;

   set_buffer=true;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_rtx :: ~pix_rtx()
{
   // clean my buffer
   delete_buffer();
}

/////////////////////////////////////////////////////////
// CreateBuffer
//
/////////////////////////////////////////////////////////
void pix_rtx :: create_buffer(imageStruct image)
{
   long dataSize = image.xsize * image.xsize * image.ysize * image.csize * sizeof(unsigned char); // only 1 channel !!, to keep data-size handy

   buffer.xsize = image.xsize;
   buffer.ysize = image.ysize;
   buffer.csize = image.csize;
#ifdef IMAGE_CLASS
   buffer.allocate( dataSize );
#else
   buffer.data = new unsigned char[dataSize];
   memset(buffer.data, 0, dataSize);
#endif
}

/////////////////////////////////////////////////////////
// DeleteBuffer
//
/////////////////////////////////////////////////////////
void pix_rtx :: delete_buffer()
{
#ifdef IMAGE_CLASS
    buffer.clear();
#else
    delete [] buffer.data;
    buffer.data = NULL;
#endif
}


/////////////////////////////////////////////////////////
// ClearBuffer
//
/////////////////////////////////////////////////////////
void pix_rtx :: clear_buffer()
{
		memset(buffer.data, 0, buffer.xsize * buffer.xsize * buffer.ysize * buffer.csize * sizeof(unsigned char));
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_rtx :: processImage(imageStruct &image)
{
  // assume that the pix_size does not change !
  if (image.xsize != buffer.xsize || image.ysize != buffer.ysize || image.csize != buffer.csize) {
    long dataSize = image.xsize * image.xsize * image.ysize * image.csize * sizeof(unsigned char);
#ifdef IMAGE_CLASS		//tigital
    buffer.clear();
    buffer.allocate( dataSize );
#else
    delete [] buffer.data;
    buffer.data = new unsigned char[dataSize];
#endif
    buffer.xsize = image.xsize;
    buffer.ysize = image.ysize;
    buffer.csize = image.csize;
    memset(buffer.data, 0, dataSize);
  }

   int pixsize = image.ysize * image.xsize;

   int cols=image.xsize, c=0, c1=0;
   int rows=image.ysize, r=0;

   unsigned char *pixels = image.data;
   unsigned char *wp;			// write pointer
   unsigned char *rp;			// read pointer

   // first copy the pixels into our buffer
   if (!set_buffer) {
     wp = buffer.data + pixsize * buffer.csize * bufcount;
     memcpy(wp, pixels, pixsize * buffer.csize * sizeof(unsigned char));
   } else { // fill the buffer with the current frame
     // this might be useful to prevent the black screen in the beginning.
     // "set" message
     c=cols;
     while (c--) {
       wp = buffer.data + pixsize * buffer.csize * c;
       memcpy(wp, pixels, pixsize * buffer.csize * sizeof(unsigned char));
       set_buffer=false;
     }
   }

   // then copy the buffer rtx-transformed back to the pixels
   while (c < cols) {
	   c1 = mode?((c+cols-bufcount)%cols):(c+1)%cols;

	   while (r < rows) {
		   rp = buffer.data + buffer.csize * (buffer.xsize * buffer.ysize * c + buffer.xsize * r + (bufcount - c + cols) % cols );
		   pixels = image.data + image.csize * (image.xsize * r + cols - c1);

		   pixels[chRed]   = rp[chRed];
		   pixels[chBlue]  = rp[chBlue];
		   pixels[chGreen] = rp[chGreen];
		   pixels[chAlpha] = rp[chAlpha];

		   r++;
	   }
	   r=0;
	   c++;
   }

   bufcount++;
   bufcount%=image.xsize;
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_rtx :: obj_setupCallback(t_class *classPtr)
{
  //class_addcreator((t_newmethod)_classpix_rtx, gensym("pix_rtx"), A_NULL);


	class_addmethod(classPtr, (t_method)&pix_rtx::modeMessCallback,
 					  gensym("mode"), A_FLOAT, A_NULL);
	class_addmethod(classPtr, (t_method)&pix_rtx::clearMessCallback,
 					  gensym("clear"), A_NULL);
	class_addmethod(classPtr, (t_method)&pix_rtx::setMessCallback,
 					  gensym("set"), A_NULL);

}
void pix_rtx :: modeMessCallback(void *data, t_floatarg newmode)
{
    GetMyClass(data)->mode=(newmode!=0);
}
void pix_rtx :: clearMessCallback(void *data)
{
    GetMyClass(data)->clear_buffer();
}
void pix_rtx :: setMessCallback(void *data)
{
  GetMyClass(data)->set_buffer = true;
}
