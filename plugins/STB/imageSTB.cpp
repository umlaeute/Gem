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
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <string.h>
#include "imageSTB.h"
#include "Gem/RTE.h"
#include "plugins/PluginFactory.h"

#ifndef HAVE_LIBSTB
# define STB_IMAGE_IMPLEMENTATION
# define STB_IMAGE_WRITE_IMPLEMENTATION
#endif
#ifdef HAVE_STB_STB_IMAGE_H
# include "stb/stb_image.h"
# include "stb/stb_image_write.h"
#else
# include "stb_image.h"
# include "stb_image_write.h"
#endif

using namespace gem::plugins;

REGISTER_IMAGELOADERFACTORY("STB", imageSTB);
REGISTER_IMAGESAVERFACTORY ("STB", imageSTB);

/////////////////////////////////////////////////////////
//
// imageSTB
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
imageSTB :: imageSTB(void)
{
}
imageSTB :: ~imageSTB(void)
{
}

/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
bool imageSTB :: load(std::string filename, imageStruct&result,
                      gem::Properties&props)
{
  int xsize, ysize, csize;
  unsigned char *data = stbi_load(filename.c_str(), &xsize, &ysize, &csize, 4);

  if(!data) {
    return(false);
  }

  result.xsize=xsize;
  result.ysize=ysize;
  result.setFormat(GEM_RGBA);
  result.reallocate();

  result.fromRGBA(data);

  stbi_image_free(data);
  return true;
}

bool imageSTB::save(const imageStruct&image, const std::string&filename,
                    const std::string&mimetype, const gem::Properties&props)
{
  int err = 0;
  imageStruct img;
  double fquality=100;
  int quality=fquality;

  if(props.get("quality", fquality)) {
    quality=fquality;
  }

  image.convertTo(&img, GEM_RAW_RGBA);

  if(!img.upsidedown) {
    stbi_flip_vertically_on_write(1);
  }
  if("image/png" == mimetype) {
    err = stbi_write_png(filename.c_str(), img.xsize, img.ysize, img.csize, img.data, img.xsize * img.csize);
  } else if ("image/bmp" == mimetype) {
    err = stbi_write_bmp(filename.c_str(), img.xsize, img.ysize, img.csize, img.data);
  } else if ("image/targa" == mimetype) {
    err = stbi_write_tga(filename.c_str(), img.xsize, img.ysize, img.csize, img.data);
  } else if ("image/jpeg" == mimetype) {
    err = stbi_write_jpg(filename.c_str(), img.xsize, img.ysize, img.csize, img.data, quality);
  }

  return (0!=err);
}
float imageSTB::estimateSave(const imageStruct&img,
                             const std::string&filename, const std::string&mimetype,
                             const gem::Properties&props)
{
  float result=0;
  if("image/png" == mimetype) {
    result+=80;
  } else if ("image/bmp" == mimetype) {
    result+=80;
  } else if ("image/targa" == mimetype) {
    result+=80;
  } else if ("image/jpeg" == mimetype) {
    result+=80;
    if(gem::Properties::UNSET != props.type("quality")) {
      result += 1.;
    }

  }
  return result;
}
void imageSTB::getWriteCapabilities(std::vector<std::string>&mimetypes,
                                    gem::Properties&props)
{
  mimetypes.clear();
  props.clear();
  mimetypes.push_back("image/png");
  mimetypes.push_back("image/bmp");
  mimetypes.push_back("image/targa");
  mimetypes.push_back("image/jpeg");
  //mimetypes.push_back("image/hdr"); // ??; expects float
  gem::any value;

  value=100.f;
  props.set("quality", value);
}
