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
//   mailto:zmoelnig@iem.at
//
//   this code is published under the Gnu GeneralPublicLicense that should be distributed with gem & pd
//
/////////////////////////////////////////////////////////

#include "pix_rtx.h"
#include <string.h>

/////////////////////////////////////////////////////////
// CreateBuffer
//
/////////////////////////////////////////////////////////
bool refresh_buffer(const imageStruct&reference, imageStruct&buffer)
{
  // only 1 channel !!, to keep data-size handy
  unsigned char*data = buffer.data;
  size_t imagesize = reference.xsize * reference.ysize * reference.csize  * sizeof(unsigned char);
  size_t dataSize = imagesize * reference.ysize;
  bool refresh=
    (reference.xsize != buffer.xsize) ||
    (reference.ysize != buffer.ysize) ||
    (reference.csize != buffer.csize);

  buffer.xsize = reference.xsize;
  buffer.ysize = reference.ysize;
  buffer.setFormat(reference.format);

  if(data!=buffer.reallocate( dataSize ) || refresh) {
    buffer.setBlack();
  }
  return (0!=buffer.data);
}



CPPEXTERN_NEW(pix_rtx);

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
  image.setFormat(GEM_RGBA);

  refresh_buffer(image, buffer);

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
  buffer.clear();
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_rtx :: processImage(imageStruct &image)
{
  if (!refresh_buffer(image, buffer)) {
    // ouch, couldn't allocate memory!
    return;
  }

  size_t pixsize = image.ysize * image.xsize;
  size_t cols=image.xsize, c=0, c1=0;
  size_t rows=image.ysize, r=0;

  unsigned char *pixels = image.data;
  unsigned char *wp;                   // write pointer
  unsigned char *rp;                   // read pointer

  // first copy the pixels into our buffer
  if (!set_buffer) {
    wp = buffer.data + pixsize * buffer.csize * bufcount;
    memcpy(wp, pixels, pixsize * buffer.csize * sizeof(unsigned char));
  } else {
    // fill the buffer with the current frame
    // this might be useful to prevent the black screen in the beginning.
    // "set" message
    c=cols;
    size_t imagesize=pixsize * buffer.csize * sizeof(unsigned char);
    while (c--) {
      wp = buffer.data + imagesize * c;
      memcpy(wp, pixels, imagesize);
    }
    c = 0;
    set_buffer=false;
  }

  // then copy the buffer rtx-transformed back to the pixels
  switch(image.csize) {
  case 1: // Grey
    while (c < cols) {
      c1 = mode?((c+cols-bufcount)%cols):(c+1)%cols;
      while (r < rows) {
        rp = buffer.data + buffer.csize * (buffer.xsize * buffer.ysize * c +
                                           buffer.xsize * r + (bufcount - c + cols) % cols );
        pixels = image.data + image.csize * (image.xsize * r + cols - c1);

        *pixels   = *rp;
        r++;
      }
      r=0;
      c++;
    }
    break;
  case 2: // YUV
    while (c < cols) {
      c1 = mode?((c+cols-bufcount)%cols):(c+1)%cols;
      while (r < rows) {
        rp = buffer.data + buffer.csize * (buffer.xsize * buffer.ysize * c +
                                           buffer.xsize * r + (bufcount + cols - c) % cols );
        pixels = image.data + image.csize * (image.xsize * r + cols - c1);

        pixels[0]  = rp[0];
        pixels[1]  = rp[1];
        r++;
      }
      r=0;
      c++;
    }
    break;
  case 4: // RGBA
    while (c < cols) {
      c1 = mode?((c+cols-bufcount)%cols):(c+1)%cols;
      while (r < rows) {
        rp = buffer.data + buffer.csize * (buffer.xsize * buffer.ysize * c +
                                           buffer.xsize * r + (bufcount - c + cols) % cols );
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
    break;
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
  CPPEXTERN_MSG1(classPtr, "mode", modeMess, int);
  CPPEXTERN_MSG0(classPtr, "clear", clearMess);
  CPPEXTERN_MSG0(classPtr, "set", setMess);
}
void pix_rtx :: modeMess(int newmode)
{
  mode=(newmode!=0);
}
void pix_rtx :: clearMess()
{
  buffer.setBlack();
}
void pix_rtx :: setMess()
{
  set_buffer = true;
}
