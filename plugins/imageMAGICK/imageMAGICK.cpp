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
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_LIBMAGICK
# include <magick/MagickCore.h>

#include <string.h>
#include "imageMAGICK.h"
#include "plugins/PluginFactory.h"

#include "Gem/RTE.h"

// hmm, the GetMimeList() function has changed!
//  ImageMagick-6.6.2-0: **GetMimeList(const char *,unsigned long *,ExceptionInfo *),
//  ImageMagick-6.6.2-1: **GetMimeList(const char *,size_t *,ExceptionInfo *),
// theoretically, "unsigned long" and "size_t" are pretty much the same
// but in practice the compiler will complain bitterly
// set let's do some magick...

#ifndef MagickLibInterface
# define MagickLibInterface 0
#endif
#ifndef MagickLibVersion
# define MagickLibVersion 0
#endif

// this won't catch ImageMagick>=6.6.2-0, but what can I do?
// ubuntu/natty ships with 6.6.2-6!
// another workaround: compile with "-fpermissive"
#if (MagickLibInterface > 3) || (MagickLibVersion >= 0x662)
# define mimelistlength_t size_t
#else
# define mimelistlength_t unsigned long
#endif


using namespace gem::plugins;


namespace MagickCore {};
using namespace MagickCore;

namespace MagickLib  {};
using namespace MagickLib;

REGISTER_IMAGELOADERFACTORY("magick", imageMAGICK);
REGISTER_IMAGESAVERFACTORY("magick", imageMAGICK);



namespace {
  static bool showException(ExceptionInfo*exception, const std::string&prefix=std::string()) {
    if(!exception)return true;
    if (exception->severity == UndefinedException) 
      return true;

    bool iswarning=exception->severity < ErrorException;

    std::string message=prefix;
    if(!prefix.empty())
      message+=": ";

    message += SetClientName(0);
    if ( exception->reason != 0 ) {
      message += std::string(": ");
      message += std::string(exception->reason);
    }
    if ( exception->description != 0 )
      message += " (" + std::string(exception->description) + ")";

    if(iswarning) {
      verbose(1, "%s", message.c_str());
    } else {
      verbose(1, "%s", message.c_str());
    }
    return iswarning;
  }
}

/////////////////////////////////////////////////////////
//
// imageMAGICK
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

imageMAGICK :: imageMAGICK(void)
{
  if(!IsMagickInstantiated())
    MagickCoreGenesis(NULL,MagickTrue);


  //post("imageMAGICK");
  char**mimelist;
  char what;
  mimelistlength_t  length;
  ExceptionInfo exception;
  GetExceptionInfo(&exception);
  mimelist=GetMimeList("image/*", &length, &exception);
  unsigned int i;
  for(i=0; i<length; i++) {
    m_mimetypes.push_back(mimelist[i]);
  }

}
imageMAGICK :: ~imageMAGICK(void)
{
  //post("~imageMAGICK");
}

/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
bool imageMAGICK :: load(std::string filename, imageStruct&result, gem::Properties&props)
{
  ::verbose(2, "reading '%s' with ImageMagick", filename.c_str());
  ExceptionInfo*exception=AcquireExceptionInfo();
  ImageInfo*image_info=CloneImageInfo((ImageInfo *) NULL); 
  CopyMagickString(image_info->filename,filename.c_str(), MaxTextExtent);

  Image*image=ReadImage(image_info,exception);
  if(!showException(exception, "magick reading problem"))
    goto failed;
  if (image == (Image *) NULL)
    goto failed;

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
  if(!showException(exception, "magick decoding problem"))
    goto failed;

  image_info=DestroyImageInfo(image_info); 
  exception=DestroyExceptionInfo(exception);
  return true;

 failed:
  image_info=DestroyImageInfo(image_info); 
  exception=DestroyExceptionInfo(exception);
  return false;
}
bool imageMAGICK::save(const imageStruct&image, const std::string&filename, const std::string&mimetype, const gem::Properties&props) {
  imageStruct*img=const_cast<imageStruct*>(&image);
  imageStruct*pImage=img;

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
  if(!showException(exception, "magick conversion problem"))
    goto failed;

  finalImage=(pImage->upsidedown)?mimage:FlipImage( mimage, exception );
  if(!showException(exception, "magick flipping problem"))
    goto failed;

  finalImage->depth=8;
  //options->depth = 8;

  double quality;
  if(props.get("quality", quality)) {
    finalImage->quality=quality;
    //options->quality = quality;
  }

  WriteImage(image_info,finalImage);
  if(!showException(&finalImage->exception, "magick writing problem"))
    goto failed;


  if(finalImage!=mimage)
    finalImage=DestroyImage(finalImage); 

  mimage=DestroyImage(mimage); 
  exception=DestroyExceptionInfo(exception); 
  image_info=DestroyImageInfo(image_info);
  
  return true;

 failed:
  if(finalImage!=mimage)
    finalImage=DestroyImage(finalImage); 

  mimage=DestroyImage(mimage); 
  exception=DestroyExceptionInfo(exception); 
  image_info=DestroyImageInfo(image_info);

  return false;
}

float imageMAGICK::estimateSave(const imageStruct&image, const std::string&filename, const std::string&mimetype, const gem::Properties&props) {
  float result=0.5; // slightly preference for MAGICK

  int i;
  for(i=0; i<m_mimetypes.size(); i++) {
    if(mimetype==m_mimetypes[i]) {
      result+=100.;
      break;
    }
  }

  if(gem::Properties::UNSET != props.type("quality"))
    result += 1.;

  return result;
}

void imageMAGICK::getWriteCapabilities(std::vector<std::string>&mimetypes, gem::Properties&props) {
  mimetypes.clear();
  props.clear();

  mimetypes = m_mimetypes;

  gem::any value;

  value=100.f;
  props.set("quality", value);
}

#endif
