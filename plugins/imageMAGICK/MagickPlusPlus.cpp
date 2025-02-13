////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

/* this implements ImageMagick loading/saving using Magick++ */

#include "imageMAGICK.h"
#include "Gem/RTE.h"

#include <Magick++.h>

using namespace gem::plugins;

namespace MagickCore {};
using namespace MagickCore;

namespace MagickLib  {};
using namespace MagickLib;

/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
bool imageMAGICK :: load(std::string filename, imageStruct&result,
                         gem::Properties&props)
{
  Magick::Image image;
  try {
    // Read a file into image object
    try {
      image.read( filename );
      image.autoOrient();
    } catch (Magick::Warning&e) {
      logpost(0, 3+0, "[GEM:imageMAGICK] loading problem: %s", e.what());
    }

    result.xsize=static_cast<int>(image.columns());
    result.ysize=static_cast<int>(image.rows());
    result.setFormat(GEM_RGBA);
    result.reallocate();

    result.upsidedown=true;

    try {
      image.write(0,0,result.xsize,result.ysize,
#ifdef __APPLE__
                  "ARGB",
#else
                  "RGBA",
#endif
                  Magick::CharPixel,
                  reinterpret_cast<void*>(result.data));
    } catch (Magick::Warning&e) {
      logpost(0, 3+0, "[GEM:imageMAGICK] decoding problem: %s", e.what());
    }
  } catch (Magick::Exception&e)  {
    logpost(0, 3+0, "[GEM:imageMAGICK] loading image failed with: %s", e.what());
    return false;
  }
  return true;
}
bool imageMAGICK::save(const imageStruct&image, const std::string&filename,
                       const std::string&mimetype, const gem::Properties&props)
{
  imageStruct*img=const_cast<imageStruct*>(&image);
  imageStruct*pImage=img;

  std::string cs;
  switch(img->format) {
  case GEM_RAW_GRAY:
    cs="K";
    break;
  case GEM_RAW_RGBA:
#ifdef __APPLE__
    cs="ABGR";
#else
    cs="RGBA";
#endif
    break;
  /* coverity[unterminated_default] */
  default:
    pImage=new imageStruct();
    pImage->convertFrom(img, GEM_RAW_RGB);
  case GEM_RAW_RGB:
    cs="RGB";
    break;
  case GEM_RAW_BGRA:
#ifdef __APPLE__
    cs="ARGB";
#else
    cs="BGRA";
#endif
    break;
  }
  try {
    Magick::Image mimage(pImage->xsize, pImage->ysize, cs, Magick::CharPixel,
                         pImage->data);
    // since openGL is upside down
    if(!pImage->upsidedown) {
      mimage.flip();
    }
    // 8 bits per channel are enough!
    // LATER make this dependent on the image->type
    mimage.depth(8);
    double quality;
    if(props.get("quality", quality)) {
      mimage.quality(quality);
    }

    try {
      // finally convert and export
      mimage.write(filename);
    } catch (Magick::Warning&e) {
      logpost(0, 3+0, "[GEM:imageMAGICK] saving problem: %s", e.what());
    }

  } catch (Magick::Exception&e) {
    logpost(0, 3+0, "[GEM:imageMAGICK] %s", e.what());
    if(pImage!=&image) {
      delete pImage;
    }
    pImage=NULL;
    return false;
  } catch (...) {
    logpost(0, 3+0, "[GEM:imageMAGICK] uncaught exception!");
    return false;
  }
  if(pImage!=&image) {
    delete pImage;
  }
  pImage=NULL;
  return true;
}
