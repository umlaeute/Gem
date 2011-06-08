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

#if defined HAVE_FRAMEWORK_QT && defined HAVE_FRAMEWORK_CARBON
#include "imageQT.h"

#include "Gem/RTE.h"


using namespace gem::plugins;

REGISTER_IMAGEFACTORY("QT", imageQT);


/////////////////////////////////////////////////////////
//
// imageQT
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

imageQT :: imageQT() 
{
  post("imageQT");
}
imageQT :: ~imageQT()
{
  post("~imageQT");
}

/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////

static bool QuickTimeImage2mem(GraphicsImportComponent inImporter, imageStruct&result)
{
  Rect      r;
  if (::GraphicsImportGetNaturalBounds(inImporter, &r)) return NULL;   //get an image size
  ::OffsetRect(&r, -r.left, -r.top);                           
  if (::GraphicsImportSetBoundsRect(inImporter, &r)) return NULL;      
  ImageDescriptionHandle imageDescH = NULL;
  if (::GraphicsImportGetImageDescription(inImporter, &imageDescH)) return NULL;
      
  result.xsize   = (*imageDescH)->width;
  result.ysize   = (*imageDescH)->height;

  OSType pixelformat = 0;

  /* afaik, there is no 8bit grayscale format....
   * and even if it was, k8GrayPixelFormat would not be a define...
   */
#ifdef k8GrayPixelFormat
  /* from the docs on "depth": what depth is this data (1-32) or ( 33-40 grayscale ) */
  if ((*imageDescH)->depth <= 32) {
    result.setCsizeByFormat(GL_RGBA_GEM);
    pixelformat = k32ARGBPixelFormat;
  } else {
    result.setCsizeByFormat(GL_LUMINANCE);
    pixelformat = k8GrayPixelFormat;
  }
#else
  result.setCsizeByFormat(GL_RGBA_GEM);
  pixelformat = k32ARGBPixelFormat;
#endif
  
  ::DisposeHandle(reinterpret_cast<Handle>(imageDescH));
  imageDescH = NULL;
  result.reallocate();

#ifdef __DEBUG__
  post("QuickTimeImage2mem() : allocate %d bytes", result.xsize*result.ysize*result.csize);
#endif
  if (result.data == NULL || err) {
    error("Can't allocate memory for an image.");
    return false;
  }

  GWorldPtr   gw = NULL;

  OSErr err = QTNewGWorldFromPtr(&gw,
                                 /* taken from pix_filmDarwin */
                                 pixelformat,   // gives noErr
                                 &r, NULL, NULL, 0,
                                 // keepLocal,   
                                 //useDistantHdwrMem, 
                                 result.data, 
                                 static_cast<long>(result.xsize * result.csize));
  ::GraphicsImportSetGWorld(inImporter, gw, NULL);
  ::GraphicsImportDraw(inImporter);
  ::DisposeGWorld(gw);         //dispose the offscreen
  gw = NULL;

  return true;
}


OSStatus
FSPathMakeFSSpec(
		 const UInt8 *path,
		 FSSpec *spec,
		 Boolean *isDirectory)   /* can be NULL */
{
  OSStatus   result;
  FSRef      ref;
  
  /* check parameters */
  require_action(NULL != spec, BadParameter, result = paramErr);
   
  /* convert the POSIX path to an FSRef */
  result = FSPathMakeRef(path, &ref, isDirectory);
  require_noerr(result, FSPathMakeRef);
   
  /* and then convert the FSRef to an FSSpec */
  result = FSGetCatalogInfo(&ref, kFSCatInfoNone, NULL, NULL, spec, NULL);
  require_noerr(result, FSGetCatalogInfo);
   
 FSGetCatalogInfo:
 FSPathMakeRef:
 BadParameter:

  return ( result );
}

bool imageQT :: load(std::string filename, imageStruct&result)
{
  OSErr            err;
  GraphicsImportComponent    importer = NULL;

  ::verbose(2, "reading '%s' with QuickTime", filename.c_str());

  // does the file even exist?
  if (filename[0] != '\0') {
    FSSpec   spec;

    err = ::FSPathMakeFSSpec( reinterpret_cast<const UInt8*>(filename), &spec, NULL);
    if (err) {
      error("GemImageLoad: Unable to find file: %s", filename.c_str());
      error("parID : %d", spec.parID); 
      return NULL;
    }
    err = ::GetGraphicsImporterForFile(&spec, &importer);
    if (err) {
      error("GemImageLoad: Unable to import an image: %#s", spec.name);
      return NULL;
    }
  }
  bool ret = QuickTimeImage2mem(importer, result);

  ::CloseComponent(importer);

  return ret;
}
bool imageQT::save(std::string filename, const imageStruct&constimage) {

  return true;
}
#endif
