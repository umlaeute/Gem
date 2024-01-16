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
#include "imageSGI.h"
#include "Gem/RTE.h"
#include "sgiimage.h"
#include "plugins/PluginFactory.h"


using namespace gem::plugins;

REGISTER_IMAGELOADERFACTORY("SGI", imageSGI);
REGISTER_IMAGESAVERFACTORY ("SGI", imageSGI);

/////////////////////////////////////////////////////////
//
// imageSGI
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
imageSGI :: imageSGI(void)
{
}
imageSGI :: ~imageSGI(void)
{
}

/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
bool imageSGI :: load(std::string filename, imageStruct&result,
                      gem::Properties&props)
{
  int32 xsize, ysize, csize;
  if (!sizeofimage(filename.c_str(), &xsize, &ysize, &csize) ) {
    return(false);
  }

  result.xsize=xsize;
  result.ysize=ysize;

  if (csize == 4 || csize == 3) {
    result.setFormat(GEM_RGBA);
  } else if (csize == 1) {
    result.setFormat(GEM_GRAY);
  } else {
    fprintf(stderr,
            "[GEM:imageSGI] unknown color components in SGI file: %s\n",
            filename.c_str());
    return(false);
  }
  result.reallocate();

  unsigned int32 *readData = longimagedata((char *)filename.c_str());
  if (!readData) {
    fprintf(stderr, "[GEM:imageSGI] error reading SGI image file: %s\n",
            filename.c_str());
    return false;
  }

  unsigned char *src = reinterpret_cast<unsigned char*>(readData);
  const int yStride = result.xsize * result.csize;
  unsigned char *dst = &(result.data[0]) + yStride * (result.ysize - 1);

  // do RGBA data
  if (csize == 4) {
    while (ysize--) {
      unsigned char *pixels = dst;
      int count = xsize;
      while(count--) {
        pixels[chRed]   = src[0];
        pixels[chGreen] = src[1];
        pixels[chBlue]  = src[2];
        pixels[chAlpha] = src[3];
        pixels += 4;
        src += 4;
      }
      dst -= yStride;
    }
  } else if (csize == 3) {
    // do RGB data
    while (ysize--) {
      unsigned char *pixels = dst;
      int count = xsize;
      while(count--) {
        pixels[chRed]   = src[0];
        pixels[chGreen] = src[1];
        pixels[chBlue]  = src[2];
        pixels[chAlpha] = 255;;
        pixels += 4;
        src += 4;
      }
      dst -= yStride;
    }
  } else  {
    // do grayscale
    while (ysize--) {
      unsigned char *pixels = dst;
      int count = xsize;
      while(count--) {
        pixels[0] = src[0];
        pixels++;
        src += 4;
      }
      dst -= yStride;
    }
  }

  free(readData);
  return true;
}

bool imageSGI::save(const imageStruct&image, const std::string&filename,
                    const std::string&mimetype, const gem::Properties&props)
{
  imageStruct img;
  image.convertTo(&img, GEM_RAW_RGBA);
  unsigned int32*data=(unsigned int32*)img.data;

  std::string name="";
  if (!props.get("imagename", name)) {
    name = std::string("Gem image");
  }

  int result=0;
  if(data) {
    result=longstoimage(data, img.xsize, img.ysize, 4, filename.c_str(),
                        name.c_str());
  }

  return (0!=result);
}
float imageSGI::estimateSave(const imageStruct&img,
                             const std::string&filename, const std::string&mimetype,
                             const gem::Properties&props)
{
  float result=0;
  if("image/sgi" == mimetype) {
    result+=100;
  } else if ("image/x-rgb" == mimetype) {
    result+=50;
  }

  if(gem::Properties::UNSET != props.type("imagename")) {
    result+=1.;
  }
  return result;
}
void imageSGI::getWriteCapabilities(std::vector<std::string>&mimetypes,
                                    gem::Properties&props)
{
  mimetypes.clear();
  props.clear();

  mimetypes.push_back("image/sgi");
  mimetypes.push_back("image/x-rgb"); // ??

  gem::any value=std::string("");
  props.set("imagename", value);
}
