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
void GemPixObj :: render(GemState *state)
{
  if (!state || !state->image || !state->image->newimage || !m_processOnOff) return;
  imageStruct &image = state->image->image;

  // We save all information of our image, so the pix-processors can do what they want:
  //   change formats, sizes, databuffer, whatever
  // the data is restored in the <postrender> call, so that the objects can rely on their (buffered)
  // images

  oldImage  = &image;
  old_data  = image.data;
  old_xsize = image.xsize;
  old_ysize = image.ysize;
  old_csize = image.csize;
  old_format= image.format;

  if (state->image->image.format == GL_BGRA_EXT || state->image->image.format == GL_RGBA )
    processRGBAImage(state->image->image);
  else if (state->image->image.format == GL_LUMINANCE)
    processGrayImage(state->image->image);
  // YCBCR_422_APPLE                 0x85B9  ???
  //  else if (state->image->image.format == GL_YCBCR_422_APPLE) // this is the real Mac-Format
  else if (state->image->image.format == GL_YCBCR_422_GEM)
    processYUVImage(state->image->image);
  else
    processImage(state->image->image);
}

//////////
// get the original state back
void GemPixObj :: postrender(GemState *state){
  if (!oldImage)return;

  oldImage->data  = old_data;
  oldImage->xsize = old_xsize;
  oldImage->ysize = old_ysize;
  oldImage->csize = old_csize;
  oldImage->format= old_format;
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
// processRGBAImage
//
/////////////////////////////////////////////////////////
void GemPixObj :: processRGBAImage(imageStruct &image)
{  processImage(image); }

/////////////////////////////////////////////////////////
// processGrayImage
//
/////////////////////////////////////////////////////////
void GemPixObj :: processGrayImage(imageStruct &image)
{  processImage(image); }

/////////////////////////////////////////////////////////
// processYUVImage
//
/////////////////////////////////////////////////////////
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
