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

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

extern "C"
{
#include "tiffio.h"

#undef EXTERN

#ifdef _WINDOWS
#undef FAR
#endif

#include "jpeglib.h"
}


#include "sgiimage.h"
#include "GemPixUtil.h"

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
  int planar_conf = PLANARCONFIG_CONTIG;
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
