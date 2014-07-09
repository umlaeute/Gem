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
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

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
typedef _w64 int        ssize_t;
# endif
#endif
#include <magick/MagickCore.h>

using namespace gem::plugins;

namespace {
  static bool showException(ExceptionInfo*exception, const std::string&prefix=std::string()) {
    if(!exception)return true;
    if (exception->severity == UndefinedException)
      return false;

    bool iswarning=exception->severity < ErrorException;

    std::string message=prefix;
    message+="[";
    message+= SetClientName(0);
    message+="]";

    if(!iswarning)
      message+"!";

    message+=": ";

    if ( exception->reason != 0 ) {
      message += std::string(exception->reason);
    }
    if ( exception->description != 0 )
      message += " (" + std::string(exception->description) + ")";

    if(iswarning) {
      verbose(1, "%s", message.c_str());
    } else {
      verbose(1, "%s", message.c_str());
    }
    return (!iswarning);
  }
}


/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
bool imageMAGICK :: load(std::string filename, imageStruct&result, gem::Properties&props)
{
  bool success=false;
  ::verbose(2, "reading '%s' with ImageMagick", filename.c_str());
  ExceptionInfo*exception=AcquireExceptionInfo();
  ImageInfo*image_info=CloneImageInfo((ImageInfo *) NULL);
  CopyMagickString(image_info->filename,filename.c_str(), MaxTextExtent);

  Image*image=ReadImage(image_info,exception);
  if(showException(exception, "magick reading problem"))
    goto cleanup;
  if (image == (Image *) NULL)
    goto cleanup;

  result.xsize=static_cast<GLint>(image->columns);
  result.ysize=static_cast<GLint>(image->rows);
  result.setCsizeByFormat(GL_RGBA);
  result.reallocate();

  result.upsidedown=true;

  ExportImagePixels(image, 0, 0, result.xsize, result.ysize,
                    "RGBA",
                    CharPixel,
                    reinterpret_cast<void*>(result.data),
                    exception);
  if(showException(exception, "magick decoding problem"))
    goto cleanup;

  success=true;

 cleanup:
  if(image)
    DestroyImage(image);
  image=NULL;
  if(image_info)
    image_info=DestroyImageInfo(image_info);
  if(exception)
    exception=DestroyExceptionInfo(exception);
  return success;
}
bool imageMAGICK::save(const imageStruct&image, const std::string&filename, const std::string&mimetype, const gem::Properties&props) {
  error("GEM::imageMAGICK::save (MagickCore) seems to be broken! we are trying to fix it");

  imageStruct*img=const_cast<imageStruct*>(&image);
  imageStruct*pImage=img;
  bool result=false;

  ImageInfo*image_info=CloneImageInfo((ImageInfo *) NULL);
  Image*finalImage=NULL;
  CopyMagickString(image_info->filename,filename.c_str(), MaxTextExtent);

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

  ExceptionInfo*exception=AcquireExceptionInfo();
  Image *mimage = ConstituteImage(pImage->xsize,pImage->ysize,
                                 cs.c_str(), CharPixel,
                                 pImage->data,exception);
  if(showException(exception, "magick conversion problem"))
    goto cleanup;

  finalImage=(pImage->upsidedown)?mimage:FlipImage( mimage, exception );
  if(showException(exception, "magick flipping problem"))
    goto cleanup;

  finalImage->depth=8;
  //options->depth = 8;

  double quality;
  if(props.get("quality", quality)) {
    finalImage->quality=quality;
    //options->quality = quality;
  }

  WriteImage(image_info,finalImage);
  if(showException(&finalImage->exception, "magick writing problem"))
    goto cleanup;

  result=true;

 cleanup:
  if(finalImage!=mimage)
    finalImage=DestroyImage(finalImage);

  mimage=DestroyImage(mimage);
  exception=DestroyExceptionInfo(exception);
  image_info=DestroyImageInfo(image_info);

  return result;
}
