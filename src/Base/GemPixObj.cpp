////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GemPixObj.h"
#include "GemCache.h"

/////////////////////////////////////////////////////////
//
// GemPixObj
//
/////////////////////////////////////////////////////////
GemPixObj :: GemPixObj() : 
  m_processOnOff(1), orgPixBlock(NULL) {
    cachedPixBlock.newimage=0;
    cachedPixBlock.newfilm =0;
}


/////////////////////////////////////////////////////////
// setPixModified
//
/////////////////////////////////////////////////////////
void GemPixObj :: setPixModified()
{
  if(m_cache)m_cache->resendImage = 1;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void GemPixObj :: render(GemState *state){
  // We save all information of our image, so the pix-processors can do what they want:
  //   change formats, sizes, databuffer, whatever
  // the data is restored in the <postrender> call,
  // so that the objects can rely on their (buffered) images
  if (!state || !state->image || !&state->image->image)return;
  cachedPixBlock.newimage=state->image->newimage;
  if (!state->image->newimage) {
    state->image = &cachedPixBlock;
  } else {
    orgPixBlock = state->image;
    cachedPixBlock.newimage = state->image->newimage;
    cachedPixBlock.newfilm = state->image->newfilm; //added for newfilm copy from cache cgc 6-21-03
    state->image->image.copy2ImageStruct(&cachedPixBlock.image);
    state->image = &cachedPixBlock;
    if (m_processOnOff){
      switch(state->image->image.format){
      case GL_RGBA:
      case GL_BGRA_EXT:
	processRGBAImage(state->image->image);
	break;
      case GL_RGB:
      case GL_BGR_EXT:
	processRGBImage(state->image->image);
	break;
      case GL_LUMINANCE:
	processGrayImage(state->image->image);
	break;
      case GL_YCBCR_422_GEM: //GL_YCBCR_422_APPLE
	processYUVImage(state->image->image);
      	break;
      default:
	processImage(state->image->image);
      }
    }
  }
}

//////////
// get the original state back
void GemPixObj :: postrender(GemState *state){
  state->image = orgPixBlock;
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void GemPixObj :: processImage(imageStruct &image)
{
  switch (image.format) {
  case GL_RGBA:
  case GL_BGRA_EXT:
    error("Gem: pix object cannot handle RGBA image");
    break;
  case GL_RGB:
  case GL_BGR_EXT:
    error("Gem: pix object cannot handle RGB image");
	break;
  case GL_LUMINANCE:
    error("Gem: pix object cannot handle Grey image");
    break;
  case GL_YCBCR_422_GEM:
    error("Gem: pix object cannot handle YUV image");
    break;
  default:
    error("Gem: pix object cannot handle this format (%x) !", image.format);
  }
}

/////////////////////////////////////////////////////////
// processImage (typed)
//
/////////////////////////////////////////////////////////
void GemPixObj :: processRGBAImage(imageStruct &image)
{  processImage(image); }
void GemPixObj :: processRGBImage(imageStruct &image)
{  processImage(image); }
void GemPixObj :: processGrayImage(imageStruct &image)
{  processImage(image); }
void GemPixObj :: processYUVImage(imageStruct &image)
{  processImage(image); }

/////////////////////////////////////////////////////////
// processOnOff
//
/////////////////////////////////////////////////////////
void GemPixObj :: processOnOff(int on)
{
    m_processOnOff = on;
    setPixModified();
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void GemPixObj :: obj_setupCallback(t_class *classPtr)
{
    class_addfloat(classPtr, (t_method)&GemPixObj::floatMessCallback);    
}
void GemPixObj :: floatMessCallback(void *data, float n)
{
    GetMyClass(data)->processOnOff((int)n);
}
