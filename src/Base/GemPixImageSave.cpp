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
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GemPixImageSave.h"

#include "m_pd.h"

#ifdef _WINDOWS
#include <io.h>
#else
#include <unistd.h>
#endif

#ifdef MACOSX
#include <Carbon/Carbon.h>
#include <QuickTime/QuickTime.h>
#include <QuickTime/ImageCompression.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <string.h>
#include <stdio.h>
#endif // MACOSX

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

extern "C"
{
#ifndef MACOSX

#include "tiffio.h"

#undef EXTERN

#ifdef _WINDOWS
#undef FAR
#endif

#include "jpeglib.h"
#endif
}



#include "GemPixUtil.h"


#ifdef MACOSX
OSStatus FSPathMaketoFSSpec(const UInt8 *path,FSSpec *spec,Boolean *isDirectory);
       
void GetAppFSSpec(FSSpec *fileSpec)
{
    FSRef processRef;
    FSCatalogInfo processInfo;
    ProcessSerialNumber psn = {0, kCurrentProcess};
    GetProcessBundleLocation(&psn, &processRef);
    FSGetCatalogInfo(&processRef, kFSCatInfoNodeFlags, &processInfo, NULL, fileSpec, NULL);
}

// MemAlloc
// Simple memory allocation wrapper
void* MemAlloc(unsigned long memsize)
{
    if(memsize <= 0)
	return NULL;
    else
	return (malloc(memsize));
}


char* CStringToPString(char *string)
{
    char *newString = MemAlloc(strlen(string) + 1);
    long i = 0;
    
    for(i = 0; i < strlen(string); i++)
	newString[i+1] = string[i];
    
    newString[0] = i;
    
    return newString;
}

GEM_EXTERN int mem2image(imageStruct* image, const char *filename, const int type)
{
    OSErr			err;
    ComponentResult		cErr 	= 0;
    
    GWorldPtr 			img	= NULL;
    GraphicsExportComponent 	geComp 	= NULL;
    Rect			r;
    
    OSType			osFileType 	= kQTFileTypeJPEG; //kQTFileTypeTIFF fot Tiff kQTFileTypeSGIImage for sgi
    FSSpec	spec;
    StringPtr			PfileName;
    
  /*  if (filename[0] != '\0') {
		

		err = FSPathMaketoFSSpec( (Str255)filename, &spec, NULL);
		if (err == noErr) {
			error("GEM: Unable to find file: %#s", spec.name);
                        error("GEM: Unable to find filename:%s", filename);
			error("parID : %d", spec.parID); 
                        error("GEM: File:%s exists!", filename);
			return NULL;
		}
		
	}*/
     
    GetAppFSSpec(&spec); 
    PfileName = CStringToPString(filename);
        
    err = FSMakeFSSpec(spec.vRefNum, spec.parID, PfileName, &spec);
    
   // post("GEM: mem2image height %d width %d bpp %d",image->ysize,image->xsize,image->csize);
    
    err = OpenADefaultComponent(GraphicsExporterComponentType, osFileType, &geComp);
    if (err != noErr)
    {
	error("ERROR: %d in OpenADefaultComponent()", err);
	return 0; // FIXME:
    }
    
    r.top = 0;
    r.left = 0;
    r.bottom = image->ysize;
    r.right = image->xsize;
    
   // ::OffsetRect(&r, -r.left, -r.top);
   //SetRect(&r,r.left,r.bottom,r.right,r.top);
     err = QTNewGWorldFromPtr(&img,  
                                    //k32RGBAPixelFormat,
                                    k32ARGBPixelFormat,
                                    &r, NULL, NULL, 0,
                                   // keepLocal,	
                                    //useDistantHdwrMem, 
                                    image->data, 
                                    (long)(image->xsize * image->csize));
                
    if (err != noErr)
    {
	error("ERROR: %d in QTNewGWorldFromPtr()", err);
	return 0; // FIXME:
    }
    
    
    // Set the input GWorld for the exporter
	cErr = GraphicsExportSetInputGWorld(geComp, img);
	if (cErr != noErr)
	{
	    error("ERROR: %d in GraphicsExportSetInputGWorld()", cErr);
	    return 0; // FIXME:
	}
    
    // Set the output file to our FSSpec
	cErr = GraphicsExportSetOutputFile(geComp, &spec);
	if (cErr != noErr)
	{
	    error("ERROR: %i in GraphicsExportSetOutputFile()", cErr);
	    return 0; // FIXME:
	}
        
        // Set the compression quality (needed for JPEG, not necessarily for other formats)
	cErr = GraphicsExportSetCompressionQuality(geComp, codecLosslessQuality);
	if (cErr != noErr)
	{
	    error("ERROR: %i in GraphicsExportSetCompressionQuality()", cErr);
	    return 0; // FIXME:
	}
        
        // Export it
	cErr = GraphicsExportDoExport(geComp, NULL);
	if (cErr != noErr)
	{
	    error("ERROR: %i in GraphicsExportDoExport()", cErr);
	    return 0; // FIXME:
	}
        
        // finally, close the component
        if (geComp != NULL)
	    CloseComponent(geComp);

    
    return 1;

}

OSStatus
FSPathMaketoFSSpec(
	const UInt8 *path,
	FSSpec *spec,
	Boolean *isDirectory)	/* can be NULL */
{
	OSStatus	result;
	FSRef		ref;
	
	/* check parameters */
	require_action(NULL != spec, BadParameter, result = paramErr);
	
	/* convert the POSIX path to an FSRef */
	result = ::FSPathMakeRef(path, &ref, isDirectory);
	require_noerr(result, FSPathMakeRef);
	
	/* and then convert the FSRef to an FSSpec */
	result = FSGetCatalogInfo(&ref, kFSCatInfoNone, NULL, NULL, spec, NULL);
	require_noerr(result, FSGetCatalogInfo);
	
FSGetCatalogInfo:
FSPathMakeRef:
BadParameter:

	return ( result );
}

#else
#include "sgiimage.h"

int mem2tiffImage(imageStruct* image, const char *filename);
int mem2jpegImage(imageStruct* image, const char *filenamem, int quality);
/***************************************************************************
 *
 * mem2image - Save an image to a file
 *
 ***************************************************************************/
GEM_EXTERN int mem2image(imageStruct* image, const char *filename, const int type)
{

  switch (type) {
  case 0:
    // write to a TIFF file
    if (mem2tiffImage(image, filename))
      return(1);
    break;
  default:
    // write a JPEG file
     if (mem2jpegImage(image, filename, type))
       return(1);
  }

  // unable to save image
  error("GEM: Unable to save image to '%s'", filename);
  return(0);
}

/***************************************************************************
 *
 * Write a TIFF image.
 *
 ***************************************************************************/
int mem2tiffImage(imageStruct *image, const char *filename)
{
  TIFF *tif = TIFFOpen(filename, "w");
  if (tif == NULL) {
    return(0);
  }

  uint32 width=image->xsize, height = image->ysize;
  short bits=8, samps=image->csize;
  int npixels = width * height;
  //int planar_conf = PLANARCONFIG_CONTIG;
  char *gemstring = "PD/GEM";


  TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
  TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
  TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits);
  TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samps);
  TIFFSetField(tif, TIFFTAG_PLANARCONFIG, 1);
  TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

  TIFFSetField(tif, TIFFTAG_SOFTWARE, gemstring);

  int yStride = image->xsize * image->csize;
  unsigned char *srcLine = &(image->data[npixels * image->csize]);
  srcLine -= yStride;

  for (uint32 row = 0; row < height; row++) {
    unsigned char *buf = srcLine;
    if (TIFFWriteScanline(tif, buf, row, 0) < 0)
      {
	error("GEM: could not write line %d to image %s", row, filename);
	TIFFClose(tif);
	delete [] buf;
	return(0);
      }
      srcLine -= yStride;
    }
  TIFFClose(tif);

  return (1);
}


/***************************************************************************
 *
 * Save a JPEG image.
 *
 * We have to do some funky error handling to keep the jpeg library
 *		from exiting on us.
 *
 ***************************************************************************/

/*****************************
 *
 *	Here is the error handler
 *
 *****************************/
struct my_error_mgr
{
  struct jpeg_error_mgr pub;	// "public" fields
  jmp_buf setjmp_buffer;	// for return to caller
};

typedef struct my_error_mgr * my_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */
METHODDEF(void) my_error_exit (j_common_ptr cinfo)
{
  // cinfo->err really points to a my_error_mgr struct, so coerce pointer
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  // Always display the message.
  // We could postpone this until after returning, if we chose.
  // (*cinfo->err->output_message) (cinfo);

  // Return control to the setjmp point
  longjmp(myerr->setjmp_buffer, 1);
}

/*
 *	Here is the main image saver
 */
int mem2jpegImage(imageStruct *image, const char *filename, int quality)
{
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  /* More stuff */
  FILE * outfile;		/* target file */
  JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
  JSAMPLE *image_buffer = image->data;
  int row_stride;		/* physical row width in image buffer */


  cinfo.err = jpeg_std_error(&jerr);

  /* Now we can initialize the JPEG compression object. */
  jpeg_create_compress(&cinfo);

  if ((outfile = fopen(filename, "wb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
    return (0);
  }
  jpeg_stdio_dest(&cinfo, outfile);

  cinfo.image_width = image->xsize; 	/* image width and height, in pixels */
  cinfo.image_height = image->ysize;
  cinfo.input_components = 3;		/* # of color components per pixel */
  cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */

  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
  jpeg_start_compress(&cinfo, TRUE);

  row_stride = image->xsize * 3;	/* JSAMPLEs per row in image_buffer */

  while (cinfo.next_scanline < cinfo.image_height) {
    /* jpeg_write_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could pass
     * more than one scanline at a time if that's more convenient.
     */
    row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
    if(jpeg_write_scanlines(&cinfo, row_pointer, 1) < 0){
      error("GEM: could not write line %d to image %s", cinfo.next_scanline, filename);
      jpeg_finish_compress(&cinfo);
      fclose(outfile);
      jpeg_destroy_compress(&cinfo);
      return(0);
    }
  }

  jpeg_finish_compress(&cinfo);
  fclose(outfile);
  jpeg_destroy_compress(&cinfo);

  return(1);
}
#endif //MACOSX

