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

/* this implements ImageMagick loading/saving using MagickCore */

#include "imageMAGICK.h"
#include "Gem/RTE.h"

#ifdef _MSC_VER
# if !defined(_W64)
#  if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
#   define _W64 __w64
#  else
#   define _W64
#  endif
# endif
# ifdef _WIN64
typedef __int64         ssize_t;
# else
typedef _w64 long        ssize_t;
# endif
#endif

#ifdef HAVE_MAGICK7
# include <MagickCore/MagickCore.h>
#else
# include <magick/MagickCore.h>
#endif

using namespace gem::plugins;

namespace
{
static bool showException(ExceptionInfo*exception,
                          const std::string&prefix=std::string())
{
  if(!exception) {
    return true;
  }
  if (exception->severity == UndefinedException) {
    return false;
  }

  bool iswarning=exception->severity < ErrorException;

  std::string message=prefix;
  message+="[";
  message+= SetClientName(0);
  message+="]";

  if(!iswarning) {
    message+"!";
  }

  message+=": ";

  if ( exception->reason != 0 ) {
    message += std::string(exception->reason);
  }
  if ( exception->description != 0 ) {
    message += " (" + std::string(exception->description) + ")";
  }

  if(iswarning) {
    verbose(0, "[GEM:imageMAGICK] %s", message.c_str());
  } else {
    verbose(0, "[GEM:imageMAGICK] %s", message.c_str());
  }
  return (!iswarning);
}
}


/////////////////////////////////////////////////////////
// really open the file! (OS dependent)
//
/////////////////////////////////////////////////////////
bool imageMAGICK :: load(std::string filename, imageStruct&result,
                         gem::Properties&props)
{
  bool success=false;
  ExceptionInfo*exception=AcquireExceptionInfo();
  ImageInfo*image_info=CloneImageInfo((ImageInfo *) NULL);
  CopyMagickString(image_info->filename,filename.c_str(), MaxTextExtent);

  Image*orientedImage = 0;
  Image*image=ReadImage(image_info,exception);
  if(showException(exception, "reading problem")) {
    goto cleanup;
  }
  if (image == (Image *) NULL) {
    goto cleanup;
  }

  orientedImage = AutoOrientImage(image, image->orientation, exception);
  if(!showException(exception, "orientation problem")) {
    if(orientedImage) {
      DestroyImage(image);
      image = orientedImage;
      orientedImage = 0;
    }
  }
  if (orientedImage) {
    DestroyImage(orientedImage);
    orientedImage = image;
  }

  result.xsize=static_cast<int>(image->columns);
  result.ysize=static_cast<int>(image->rows);
  result.setFormat(GEM_RGBA);
  result.reallocate();

  result.upsidedown=true;

  ExportImagePixels(image, 0, 0, result.xsize, result.ysize,
#ifdef __APPLE__
                    "ARGB",
#else
                    "RGBA",
#endif
                    CharPixel,
                    reinterpret_cast<void*>(result.data),
                    exception);
  if(showException(exception, "decoding problem")) {
    goto cleanup;
  }

  success=true;

cleanup:
  if(image) {
    DestroyImage(image);
  }
  image=NULL;
  if(image_info) {
    image_info=DestroyImageInfo(image_info);
  }
  if(exception) {
    exception=DestroyExceptionInfo(exception);
  }
  return success;
}
bool imageMAGICK::save(const imageStruct&image, const std::string&filename,
                       const std::string&mimetype, const gem::Properties&props)
{
  MagickBooleanType status = MagickFalse;
  imageStruct*img=const_cast<imageStruct*>(&image);
  imageStruct*pImage=img;
  ImageInfo*image_info=CloneImageInfo((ImageInfo *) NULL);
  Image*finalImage=NULL;

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

  ExceptionInfo*ex = 0;
  ExceptionInfo*exception=AcquireExceptionInfo();
  Image *mimage = ConstituteImage(pImage->xsize, pImage->ysize,
                                  cs.c_str(), CharPixel,
                                  pImage->data, exception);
  if(showException(exception, "conversion problem")) {
    goto cleanup;
  }

  finalImage=(pImage->upsidedown)?mimage:FlipImage( mimage, exception );
  if(showException(exception, "flipping problem")) {
    goto cleanup;
  }

  finalImage->depth=8;
  image_info->depth = 8;

  double quality;
  if(props.get("quality", quality)) {
    finalImage->quality=quality;
    image_info->quality = quality;
  }

  ex = exception;
  CopyMagickString(image_info->filename, filename.c_str(), MaxTextExtent);
  CopyMagickString(finalImage->filename, filename.c_str(), MaxTextExtent);
  SetImageInfo(image_info, 0, ex);

#ifdef HAVE_MAGICK7
  status = WriteImage(image_info, finalImage, ex);
#else
  status = WriteImage(image_info, finalImage);
  ex = &finalImage->exception;
#endif
  if(showException(ex, "writing problem")) {
    goto cleanup;
  }

cleanup:
  if(finalImage!=mimage) {
    finalImage=DestroyImage(finalImage);
  }

  mimage=DestroyImage(mimage);
  exception=DestroyExceptionInfo(exception);
  image_info=DestroyImageInfo(image_info);

  return (status == MagickTrue);
}
