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

#include "pix_flip.h"

CPPEXTERN_NEW_WITH_ONE_ARG(pix_flip, t_symbol*, A_DEFSYM);

/////////////////////////////////////////////////////////
//
// pix_flip
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_flip :: pix_flip(t_symbol*s)
  : m_flip(NONE)
{
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("symbol"),
            gensym("flip"));
  flipMess(s);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_flip :: ~pix_flip()
{ }

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_flip :: processRGBAImage(imageStruct &image)
{
  // eventually should do this inline, but in the interest of getting it done...
  imageStruct tempImg;
  if (image.data==0) {
    return;
  }
  image.copy2Image(&tempImg);
  int ySrcStride = image.xsize * image.csize;
  int yDstStride = image.xsize * image.csize;
  int xSrcStride = image.csize;
  int xDstStride = image.csize;
  unsigned char *srcLine = tempImg.data;
  unsigned char *dstLine = image.data;

  FlipType flip=m_flip;
  if(!image.upsidedown) {
    switch(flip) {
    case(HORIZONTAL) :
      flip=BOTH;
      break;
    case(VERTICAL)   :
      flip=NONE;
      break;
    case(BOTH)       :
      flip=HORIZONTAL;
      break;
    case(NONE)       :
      flip=VERTICAL  ;
      break;
    default   :
      break;
    }
  }
  image.upsidedown=true;

  switch(flip) {
  case(HORIZONTAL):
    srcLine = tempImg.data + ySrcStride - xSrcStride;
    xSrcStride = -xSrcStride;
    break;
  case(VERTICAL):
    srcLine = tempImg.data + ySrcStride * image.ysize - ySrcStride;
    ySrcStride = -ySrcStride;
    break;
  case(BOTH):
    srcLine = tempImg.data + ySrcStride * image.ysize - xSrcStride;
    xSrcStride = -xSrcStride;
    ySrcStride = -ySrcStride;
    break;
  default:
    return;
    // break;
  }
  int ySize = image.ysize;
  int xHold = image.xsize;

  while(ySize--) {
    unsigned char *srcPixels = srcLine;
    unsigned char *dstPixels = dstLine;
    int xSize = xHold;
    while(xSize--)        {
      dstPixels[chRed] = srcPixels[chRed];
      dstPixels[chGreen] = srcPixels[chGreen];
      dstPixels[chBlue] = srcPixels[chBlue];
      dstPixels[chAlpha] = srcPixels[chAlpha];
      dstPixels += xDstStride;
      srcPixels += xSrcStride;
    }
    dstLine += yDstStride;
    srcLine += ySrcStride;
  }
}

/////////////////////////////////////////////////////////
// processYUVImage
//
/////////////////////////////////////////////////////////
void pix_flip :: processYUVImage(imageStruct &image)
{
  // eventually should do this inline, but in the interest of getting it done...
  imageStruct tempImg;
  if (image.data==0) {
    return;
  }
  image.copy2Image(&tempImg);

  int ySrcStride = image.xsize * image.csize;
  int yDstStride = image.xsize * image.csize;
  int xSrcStride = image.csize*2;
  int xDstStride = image.csize*2;
  unsigned char *srcLine = tempImg.data;
  unsigned char *dstLine = image.data;

  FlipType flip=m_flip;
  if(!image.upsidedown) {
    switch(flip) {
    case(HORIZONTAL) :
      flip=BOTH;
      break;
    case(VERTICAL)   :
      flip=NONE;
      break;
    case(BOTH)       :
      flip=HORIZONTAL;
      break;
    case(NONE)       :
      flip=VERTICAL  ;
      break;
    default   :
      break;
    }
  }
  image.upsidedown=true;

  switch(flip) {
  case(HORIZONTAL):
    srcLine = tempImg.data + ySrcStride - xSrcStride;
    xSrcStride = -xSrcStride;
    break;
  case(VERTICAL):
    srcLine = tempImg.data + ySrcStride * image.ysize - ySrcStride;
    ySrcStride = -ySrcStride;
    break;
  case(BOTH):
    srcLine = tempImg.data + ySrcStride * image.ysize - xSrcStride;
    xSrcStride = -xSrcStride;
    ySrcStride = -ySrcStride;
    break;
  default:
    return;
    // break;
  }
  int ySize = image.ysize;
  int xHold = image.xsize/2;

  const int chY0x=(m_flip==VERTICAL)?chY0:chY1;
  const int chY1x=(m_flip==VERTICAL)?chY1:chY0;

  while(ySize--)    {
    unsigned char *srcPixels = srcLine;
    unsigned char *dstPixels = dstLine;
    int xSize = xHold;
    while(xSize--)        {
      dstPixels[chU] = srcPixels[chU];
      dstPixels[chY0] = srcPixels[chY0x];
      dstPixels[chV] = srcPixels[chV];
      dstPixels[chY1] = srcPixels[chY1x];
      dstPixels += xDstStride;
      srcPixels += xSrcStride;
    }
    dstLine += yDstStride;
    srcLine += ySrcStride;
  }
}
void pix_flip :: processGrayImage(imageStruct &image)
{
  // eventually should do this inline, but in the interest of getting it done...
  imageStruct tempImg;
  if (image.data==0) {
    return;
  }
  image.copy2Image(&tempImg);

  int ySrcStride = image.xsize * image.csize;
  int yDstStride = image.xsize * image.csize;
  int xSrcStride = image.csize;
  int xDstStride = image.csize;
  unsigned char *srcLine = tempImg.data;
  unsigned char *dstLine = image.data;

  FlipType flip=m_flip;
  if(!image.upsidedown) {
    switch(flip) {
    case(HORIZONTAL) :
      flip=BOTH;
      break;
    case(VERTICAL)   :
      flip=NONE;
      break;
    case(BOTH)       :
      flip=HORIZONTAL;
      break;
    case(NONE)       :
      flip=VERTICAL  ;
      break;
    default   :
      break;
    }
  }
  image.upsidedown=true;

  switch(flip) {
  case(HORIZONTAL):
    srcLine = tempImg.data + ySrcStride - xSrcStride;
    xSrcStride = -xSrcStride;
    break;
  case(VERTICAL):
    srcLine = tempImg.data + ySrcStride * image.ysize - ySrcStride;
    ySrcStride = -ySrcStride;
    break;
  case(BOTH):
    srcLine = tempImg.data + ySrcStride * image.ysize - xSrcStride;
    xSrcStride = -xSrcStride;
    ySrcStride = -ySrcStride;
    break;
  default:
    return;
    // break;
  }
  int ySize = image.ysize;
  int xHold = image.xsize;

  while(ySize--) {
    unsigned char *srcPixels = srcLine;
    unsigned char *dstPixels = dstLine;
    int xSize = xHold;
    while(xSize--)        {
      dstPixels[chGray] = srcPixels[chGray];
      dstPixels += xDstStride;
      srcPixels += xSrcStride;
    }
    dstLine += yDstStride;
    srcLine += ySrcStride;
  }
}
/////////////////////////////////////////////////////////
// flipMess
//
/////////////////////////////////////////////////////////
void pix_flip :: flipMess(FlipType type)
{
  m_flip = type;
  setPixModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_flip :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&pix_flip::horMessCallback),
                  gensym("horizontal"), A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&pix_flip::vertMessCallback),
                  gensym("vertical"), A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&pix_flip::bothMessCallback),
                  gensym("both"), A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&pix_flip::noneMessCallback),
                  gensym("none"), A_NULL);
  CPPEXTERN_MSG1(classPtr, "flip", flipMess, t_symbol*);
}
void pix_flip :: horMessCallback(void *data)
{
  GetMyClass(data)->flipMess(HORIZONTAL);
}
void pix_flip :: vertMessCallback(void *data)
{
  GetMyClass(data)->flipMess(VERTICAL);
}
void pix_flip :: bothMessCallback(void *data)
{
  GetMyClass(data)->flipMess(BOTH);
}
void pix_flip :: noneMessCallback(void *data)
{
  GetMyClass(data)->flipMess(NONE);
}
void pix_flip :: flipMess(t_symbol*s)
{
  char c=*s->s_name;
  switch(c) {
  case 'v':
  case 'V':
    flipMess(VERTICAL);
    break;
  case 'h':
  case 'H':
    flipMess(HORIZONTAL);
    break;
  case 'b':
  case 'B':
    flipMess(BOTH);
    break;
  default:
    flipMess(NONE);
  }
}
