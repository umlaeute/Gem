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
#include "Base/config.h"

#ifdef _WINDOWS
#include <io.h>
#else
#include <unistd.h>
#endif

#ifdef __APPLE__
#include <Carbon/Carbon.h>
#include <QuickTime/QuickTime.h>
#include <QuickTime/ImageCompression.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h> 
#endif // __APPLE__

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

extern "C"
{
#ifndef __APPLE__
# ifdef HAVE_LIBTIFF
#  include "tiffio.h"
# endif

# undef EXTERN

# ifdef _WINDOWS
#  undef FAR
# endif
# ifdef HAVE_LIBJPEG
#  include "jpeglib.h"
# endif
#endif /* APPLE */
}

#include "GemPixUtil.h"


#ifdef __APPLE__
extern OSStatus FSPathMakeFSSpec(const UInt8 *path,FSSpec *spec,Boolean *isDirectory);
       
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


unsigned char* CStringToPString(char *string)
{
    unsigned char *newString = (unsigned char*)MemAlloc(strlen(string) + 1);
    int i = 0;
    
    for(i = 0; i < (int)strlen(string); i++)
	newString[i+1] = string[i];
    
    newString[0] = i;
    
    return newString;
}


void InvertGLImage( unsigned char *imageData, unsigned char * outData, long imageSize, long rowBytes )
{
    long i, j;
    // This is not an optimized routine!

    // Copy rows into tmp buffer one at a time, reversing their order
    for (i = 0, j = imageSize - rowBytes; i < imageSize; i += rowBytes, j -= rowBytes) {
        memcpy( &outData[j], &imageData[i], (size_t) rowBytes );
    }

}

GEM_EXTERN int mem2image(imageStruct* image, const char *filename, const int type)
{
    OSErr			err;
    ComponentResult		cErr 	= 0;
    
    GWorldPtr 			img	= NULL;
    GraphicsExportComponent 	geComp 	= NULL;
    Rect			r;
    
    OSType			osFileType; 	//= kQTFileTypeJPEG; //kQTFileTypeTIFF fot Tiff kQTFileTypeSGIImage for sgi
    FSSpec			spec;
    FSRef			ref;


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
    
    err = ::FSPathMakeRef((UInt8*)filename, &ref,NULL );

    if (err == fnfErr) {
        // if the file does not yet exist, then let's create the file
        int fd;
                fd = open(filename, O_CREAT | O_RDWR, 0600);
                if (fd < 0)
                    return 0;
                        write(fd, " ", 1);
                        close(fd);
                                err = FSPathMakeRef((UInt8*)filename, &ref, NULL);
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

  //  err = FSMakeFSSpec(spec.vRefNum, 0, filename, &spec); //spits out -37 error but still works?
    err = FSMakeFSSpec(spec.vRefNum, spec.parID, (UInt8*)filename, &spec);  //this always gives an error -37 ???

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
        if (osFileType 	== kQTFileTypeJPEG){
         //   cErr = GraphicsExportSetCompressionQuality(geComp, codecLosslessQuality);
            cErr = GraphicsExportSetCompressionQuality(geComp,  codecHighQuality);

            if (cErr != noErr)
            {
                error("ERROR: %i in GraphicsExportSetCompressionQuality()", cErr);
                return 0; // FIXME:
            }
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

#else
#include "sgiimage.h"

#ifdef HAVE_LIBMAGICKPLUSPLUS
# include <Magick++.h>
int mem2magickImage(imageStruct* image, const char *filename);
#endif

#ifdef HAVE_LIBTIFF
int mem2tiffImage(imageStruct* image, const char *filename);
#endif
#ifdef HAVE_LIBJPEG
int mem2jpegImage(imageStruct* image, const char *filenamem, int quality);
#endif
/***************************************************************************
 *
 * mem2image - Save an image to a file
 *
 ***************************************************************************/
GEM_EXTERN int mem2image(imageStruct* image, const char *filename, const int type)
{

  switch (type) {
  case 0:
#ifdef HAVE_LIBMAGICKPLUSPLUS
    if (mem2magickImage(image, filename)) return(1);else
#endif
#ifdef HAVE_LIBTIFF
    // write to a TIFF file
    if (mem2tiffImage(image, filename))
      return(1);
#endif
    break;
  default:
#ifdef HAVE_LIBJPEG
    // write a JPEG file
     if (mem2jpegImage(image, filename, type))
       return(1);
     else
#endif
#ifdef HAVE_LIBMAGICKPLUSPLUS
    if (mem2magickImage(image, filename)) return(1);else
#endif
     break;
  }

  // unable to save image
  error("GEM: Unable to save image to '%s'", filename);
  return(0);
}
#ifdef HAVE_LIBTIFF
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
#endif /* HAVE_LIBTIFF */
#ifdef HAVE_LIBJPEG
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
#endif /* HAVE_LIBJPEG */
#endif //__APPLE__

#ifdef HAVE_LIBMAGICKPLUSPLUS
/***************************************************************************
 *
 * Write an image using ImageMagick++
 *
 ***************************************************************************/
int mem2magickImage(imageStruct *image, const char *filename)
{
  // LATER: think about writing RGBA
  try{
    Magick::Image mimage(image->xsize, image->ysize, "RGB", Magick::CharPixel, image->data);
    mimage.flip(); // since openGL is upside down
    mimage.write(filename);
  } catch (Magick::Exception e){
    error("%s", e.what());
    return 0;
  }
  return 1;
}
#endif
