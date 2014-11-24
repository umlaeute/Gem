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

#if defined HAVE_GEM_LIB_IMAGEMAGICK__  || defined HAVE_GEM_LIB_MAGICKCORE

#include <string.h>
#include "imageMAGICK.h"
#include "plugins/PluginFactory.h"
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

#ifdef HAVE_MAGICK_MAGICKCORE_H
# include <magick/MagickCore.h>
#else
# include <Magick++.h>
#endif

#ifndef HAVE_ISMAGICKINSTANTIATED
# define USE_GRAPHICSMAGICK
#endif

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
#ifdef HAVE_ISMAGICKINSTANTIATED
  if(!IsMagickInstantiated())MagickCoreGenesis(NULL,MagickTrue);
#else
  InitializeMagick(0);
#endif

  char**mimelist=0;
  mimelistlength_t  length=0;
  ExceptionInfo exception;
  GetExceptionInfo(&exception);
#ifndef USE_GRAPHICSMAGICK
  mimelist=GetMimeList("image/*", &length, &exception);
#endif
  unsigned int i;
  for(i=0; i<length; i++) {
    m_mimetypes.push_back(mimelist[i]);
  }

}
imageMAGICK :: ~imageMAGICK(void)
{
  //post("~imageMAGICK");
}



float imageMAGICK::estimateSave(const imageStruct&image, const std::string&filename, const std::string&mimetype, const gem::Properties&props) {
  float result=0.5; // slightly preference for MAGICK
  unsigned int i;
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
