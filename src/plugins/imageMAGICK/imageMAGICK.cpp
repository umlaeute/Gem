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
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_LIBMAGICKPLUSPLUS
# include <Magick++.h>

#include <string.h>
#include "imageMAGICK.h"

#include "Gem/RTE.h"


using namespace gem::plugins;

REGISTER_IMAGEFACTORY("magick", imageMAGICK);


/////////////////////////////////////////////////////////
//
// imageMAGICK
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

imageMAGICK :: imageMAGICK() 
{
  post("imageMAGICK");
}
imageMAGICK :: ~imageMAGICK()
{
  post("~imageMAGICK");
}

/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
bool imageMAGICK :: load(std::string filename, imageStruct&result)
{
  Magick::Image image;
  try {
    ::verbose(2, "reading '%s' with ImageMagick", filename.c_str());
    // Read a file into image object
    try {
      image.read( filename );
    } catch (Magick::Warning e) {
      verbose(1, "magick loading problem: %s", e.what());
    }

    result.xsize=static_cast<GLint>(image.columns());
    result.ysize=static_cast<GLint>(image.rows());
    result.setCsizeByFormat(GL_RGBA);
    result.reallocate();

    result.upsidedown=true;

    try {
      image.write(0,0,result.xsize,result.ysize, 
                  "RGBA",
                  Magick::CharPixel,
                  reinterpret_cast<void*>(result.data));
    } catch (Magick::Warning e) {
      verbose(1, "magick decoding problem: %s", e.what());
    }
  }catch( Magick::Exception e )  {
    verbose(1, "magick loading image failed with: %s", e.what());
    return false;
  }
  return true;
}
bool imageMAGICK::save(std::string filename, const imageStruct&image) {
  imageStruct*img=const_cast<imageStruct*>(&image);
  imageStruct*pImage=img;
  std::string cs;
  switch(img->format) {
  case GL_LUMINANCE:
    cs="K";
    break;
  case GL_RGBA:
    cs="RGBA";
    break;
  default:
    pImage=new imageStruct();
    pImage->convertFrom(img, GL_RGB);
  case GL_RGB:
    cs="RGB";
    break;
  case GL_BGRA_EXT:
    cs="BGRA";
    break;
  }

  try{
    Magick::Image mimage(pImage->xsize, pImage->ysize, cs, Magick::CharPixel, pImage->data);
    // since openGL is upside down
    if(!pImage->upsidedown) {
      mimage.flip();
    }
    // 8 bits per channel are enough!
    // LATER make this dependent on the image->type
    mimage.depth(8); 
    // finally convert and export
    mimage.write(filename);
  } catch (Magick::Exception e){
    error("%s", e.what());
    if(pImage!=&image)delete[]pImage; pImage=NULL;
    return false;
  }
  if(pImage!=&image)delete[]pImage; pImage=NULL;

  return true;
}
#endif
