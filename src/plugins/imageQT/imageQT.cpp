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

#if defined HAVE_GEM_FRAMEWORK_QUICKTIME && defined HAVE_GEM_FRAMEWORK_CARBON

# include "Gem/RTE.h"
# include "imageQT.h"

# include <Carbon/Carbon.h>
# include <QuickTime/QuickTime.h>
# include <QuickTime/ImageCompression.h>

//# include <string.h>
//# include <fcntl.h> 

using namespace gem::plugins;

REGISTER_IMAGEFACTORY("QT", imageQT);



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

static void GetAppFSSpec(FSSpec *fileSpec)
{
  FSRef processRef;
  FSCatalogInfo processInfo;
  ProcessSerialNumber psn = {0, kCurrentProcess};
  GetProcessBundleLocation(&psn, &processRef);
  FSGetCatalogInfo(&processRef, kFSCatInfoNodeFlags, &processInfo, NULL, fileSpec, NULL);
}

// MemAlloc
// Simple memory allocation wrapper
static void* MemAlloc(unsigned long memsize)
{
  if(memsize <= 0)
    return NULL;
  else
    return (malloc(memsize));
}

unsigned char* CStringToPString(char *string)
{
  unsigned char *newString = (unsigned char*)MemAlloc(strlen(string) + 1);
  size_t i = 0;
    
  for(i = 0; i < strlen(string); i++)
    newString[i+1] = string[i];
    
  newString[0] = i;
    
  return newString;
}


void InvertGLImage( unsigned char *imageData, unsigned char * outData, long imageSize, long rowBytes )
{
  long i, j;
  // This is not an optimized routine!

  // FIXXME use a flip function in GemPixUtils for this

  // Copy rows into tmp buffer one at a time, reversing their order
  for (i = 0, j = imageSize - rowBytes; i < imageSize; i += rowBytes, j -= rowBytes) {
    memcpy( &outData[j], &imageData[i], static_cast<size_t>(rowBytes) );
  }
}


/////////////////////////////////////////////////////////
//
// imageQT
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

imageQT :: imageQT() : image(false)
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
  if (result.data == NULL) {
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
  if (err) {
    error("Can't create QTNewWorld");
  }

  ::GraphicsImportSetGWorld(inImporter, gw, NULL);
  ::GraphicsImportDraw(inImporter);
  ::DisposeGWorld(gw);         //dispose the offscreen
  gw = NULL;

  return true;
}

bool imageQT :: load(std::string filename, imageStruct&result)
{
  OSErr            err;
  GraphicsImportComponent    importer = NULL;

  ::verbose(2, "reading '%s' with QuickTime", filename.c_str());

  // does the file even exist?
  if (filename[0] != '\0') {
    FSSpec   spec;

    err = ::FSPathMakeFSSpec( reinterpret_cast<const UInt8*>(filename.c_str()), &spec, NULL);
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

  OSErr			err;
  ComponentResult		cErr 	= 0;
    
  GWorldPtr 			img	= NULL;
  GraphicsExportComponent 	geComp 	= NULL;
  Rect			r;
    
  OSType			osFileType; 	//= kQTFileTypeJPEG; //kQTFileTypeTIFF fot Tiff kQTFileTypeSGIImage for sgi
  FSSpec			spec;
  FSRef			ref;

  unsigned char *data = NULL;

  const UInt8*filename8=reinterpret_cast<const UInt8*>(filename.c_str());

#warning output format
  int type=1;


  if(!constimage.upsidedown) { // the image is openGL-oriented, not quicktime-oriented! flip it!
    int rowBytes = constimage.xsize * constimage.csize;
    int imageSize = constimage.ysize * rowBytes;

    data = new unsigned char[imageSize];
      
    InvertGLImage(constimage.data, data, imageSize, rowBytes);
  }

  switch (type){
  case 0:
    osFileType 	=kQTFileTypeTIFF;
    break;
  case 1:
    osFileType 	= kQTFileTypeJPEG;
    break;
  default:
    osFileType 	=kQTFileTypeTIFF;
    break;
  }
    
  err = ::FSPathMakeRef(filename8, &ref, NULL );

  if (err == fnfErr) {
    // if the file does not yet exist, then let's create the file
    int fd;
    fd = open(filename.c_str(), O_CREAT | O_RDWR, 0600);
    if (fd < 0)
      return 0;
    write(fd, " ", 1);
    close(fd);
    err = FSPathMakeRef(filename8, &ref, NULL);
  }
    
  if (err != noErr)
    {
      error("ERROR: %d in FSPathMakeRef()", err);
    }

  err = ::FSGetCatalogInfo(&ref, kFSCatInfoNodeFlags, NULL, NULL, &spec, NULL);

  if (err != noErr)
    {
      error("ERROR: %d in FSGetCatalogInfo()", err);
    }

  //  err = FSMakeFSSpec(spec.vRefNum, 0, filename.c_str(), &spec); //spits out -37 error but still works?
  err = FSMakeFSSpec(spec.vRefNum, spec.parID, filename8, &spec);  //this always gives an error -37 ???

  if (err != noErr && err != -37)
    {
      error("ERROR: %d in FSMakeFSSpec()", err);
    }
        
  err = OpenADefaultComponent(GraphicsExporterComponentType, osFileType, &geComp);
  if (err != noErr)
    {
      error("ERROR: %d in OpenADefaultComponent()", err);
      return 0; // FIXME:
    }
    
  r.top = 0;
  r.left = 0;
  r.bottom = constimage.ysize;
  r.right = constimage.xsize;
    
  // ::OffsetRect(&r, -r.left, -r.top);
  //SetRect(&r,r.left,r.bottom,r.right,r.top);
  err = QTNewGWorldFromPtr(&img,  
			   //k32RGBAPixelFormat,
			   k32ARGBPixelFormat,
			   &r, NULL, NULL, 0,
			   // keepLocal,	
			   //useDistantHdwrMem, 
			   (data?data:constimage.data),
			   static_cast<long>(constimage.xsize * constimage.csize));

  // is this the right place to free the "data" buffer (if used)?
  // i don't know, whether quicktime still needs the buffer...
                
  if (err != noErr)
    {
      error("ERROR: %d in QTNewGWorldFromPtr()", err);
      if(data)delete[]data;
      return 0; // FIXME:
    }
    
    
  // Set the input GWorld for the exporter
  cErr = GraphicsExportSetInputGWorld(geComp, img);
  if (cErr != noErr)
    {
      error("ERROR: %d in GraphicsExportSetInputGWorld()", cErr);
      if(data)delete[]data;
      return 0; // FIXME:
    }
    
  // Set the output file to our FSSpec
  cErr = GraphicsExportSetOutputFile(geComp, &spec);
  if (cErr != noErr)
    {
      error("ERROR: %i in GraphicsExportSetOutputFile()", cErr);
      if(data)delete[]data;
      return 0; // FIXME:
    }
        
  // Set the compression quality (needed for JPEG, not necessarily for other formats)
  if (osFileType 	== kQTFileTypeJPEG){
    //   cErr = GraphicsExportSetCompressionQuality(geComp, codecLosslessQuality);
    cErr = GraphicsExportSetCompressionQuality(geComp,  codecHighQuality);

    if (cErr != noErr)
      {
	error("ERROR: %i in GraphicsExportSetCompressionQuality()", cErr);
	if(data)delete[]data;
	return 0; // FIXME:
      }
  }
        
  // Export it
  cErr = GraphicsExportDoExport(geComp, NULL);
  if (cErr != noErr)
    {
      error("ERROR: %i in GraphicsExportDoExport()", cErr);
      if(data)delete[]data;
      return 0; // FIXME:
    }
        
  // finally, close the component
  if (geComp != NULL)
    CloseComponent(geComp);

  if(data)delete[]data;


  return true;
}
#endif
