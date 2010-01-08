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
//    Copyright (c) 2002-2003 james tittle/tigital
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Base/GemConfig.h"
#include "Base/GemGL.h"

#include "GemPixImageLoad.h"
#include "m_pd.h"

#ifdef __APPLE__
# include <Carbon/Carbon.h>
# include <QuickTime/QuickTime.h>
#endif // __APPLE__

#ifdef _WIN32
# include <io.h>
# define close _close
#else
# include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>

#ifndef __APPLE__
extern "C"
{
#ifdef HAVE_LIBTIFF
# include "tiffio.h"
#endif

#undef EXTERN

#ifdef _WIN32
# undef FAR
#endif

#ifdef HAVE_LIBJPEG
# include "jpeglib.h"
#endif
}

#include "sgiimage.h"
#endif // __APPLE__

#include "GemPixUtil.h"

#ifdef HAVE_LIBMAGICKPLUSPLUS
# include <Magick++.h>
imageStruct *magickImage2mem(const char *filename);
#endif


#ifdef __APPLE__
imageStruct *QTImage2mem(GraphicsImportComponent inImporter);
OSStatus FSPathMakeFSSpec(
   const UInt8 *path,
   FSSpec *spec,
   Boolean *isDirectory);
#else
# ifdef HAVE_LIBTIFF
imageStruct *tiffImage2mem(const char *filename);
# endif
# ifdef HAVE_LIBJPEG
imageStruct *jpegImage2mem(const char *filename);
# endif /* LIBJPEG */
imageStruct *sgiImage2mem(const char *filename);

#endif // __APPLE__

/***************************************************************************
 *
 * image2mem - Read in an image in various file formats
 *
 ***************************************************************************/
#ifdef __APPLE__
GEM_EXTERN imageStruct *image2mem(const char *filename)
{
   OSErr            err;
   imageStruct          *image_block = NULL;
   GraphicsImportComponent    importer = NULL;

   ::verbose(2, "reading '%s' with QuickTime", filename);

   // does the file even exist?
   if (filename[0] != '\0') {
      FSSpec   spec;

      err = ::FSPathMakeFSSpec( reinterpret_cast<const UInt8*>(filename), &spec, NULL);
      if (err) {
         error("GemImageLoad: Unable to find file: %#s", spec.name);
                        error("GemImageLoad: Unable to find filename:%s", filename);
         error("parID : %d", spec.parID); 
         return NULL;
      }
      err = ::GetGraphicsImporterForFile(&spec, &importer);
      if (err) {
         error("GemImageLoad: Unable to import an image: %#s", spec.name);
         return NULL;
      }
   }
   image_block = QTImage2mem(importer);
   ::CloseComponent(importer);
   if (image_block)   return image_block;
   else         return NULL;
}
/*****************************************************************************/

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

#else
GEM_EXTERN imageStruct *image2mem(const char *filename)
{
   imageStruct *image_block = NULL;
   char newName[256];

   ::verbose(2, "image2mem(%s)", filename);

   // does the file even exist?
   if (filename[0] == '/' || filename[0] == '\\')
   {
      strcpy(newName, filename);
   }
   else
   {
     char realName[256];
     char *realResult;
     
     int fd = open_via_path(".", filename, "", realName, &realResult, 256, 1);
     if (fd < 0)
       {
         error("GemImageLoad: Unable to find file: %s", filename);
         return(NULL);
       }
     else
       {
         int res = close(fd);
         if (res == -1)
      {
        verbose(1, "GemImageLoad: Unable to close file-handle %d for '%s'", fd, filename);
      }
       }
     
     sprintf(newName, "%s/%s", realName, realResult);
   }

# ifdef HAVE_LIBMAGICKPLUSPLUS
   // try to load via ImageMagick
   if ( (image_block = magickImage2mem(newName)) )
         return(image_block);
# endif

#ifdef HAVE_LIBJPEG
   // try to load in a JPEG file
   if ( (image_block = jpegImage2mem(newName)) )
         return(image_block);
#endif
   // try to load in an SGI file
   if ( (image_block = sgiImage2mem(newName)) )
         return(image_block);
#ifdef HAVE_LIBTIFF
   // try to load in a TIFF file
   if ( (image_block = tiffImage2mem(newName)) )
         return(image_block);
#endif
   // unable to load image
   return(NULL);
}
#endif // __APPLE__
/***************************************************************************
 *
 * Read in a image utilizing QuickTime GraphicsImporterComponent
 *
 ***************************************************************************/
#ifdef __APPLE__
imageStruct *QTImage2mem(GraphicsImportComponent inImporter)
{
   Rect      r;
   if (::GraphicsImportGetNaturalBounds(inImporter, &r)) return NULL;   //get an image size
   ::OffsetRect(&r, -r.left, -r.top);                           
   if (::GraphicsImportSetBoundsRect(inImporter, &r)) return NULL;      
   ImageDescriptionHandle imageDescH = NULL;
   if (::GraphicsImportGetImageDescription(inImporter, &imageDescH)) return NULL;
      
   imageStruct *image_block = new imageStruct;
   image_block->xsize   = (*imageDescH)->width;
   image_block->ysize   = (*imageDescH)->height;

        OSType pixelformat = 0;

       /* afaik, there is no 8bit grayscale format....
        * and even if it was, k8GrayPixelFormat would not be a define...
        */
#ifdef k8GrayPixelFormat
       /* from the docs on "depth": what depth is this data (1-32) or ( 33-40 grayscale ) */
   if ((*imageDescH)->depth <= 32) {
       image_block->setCsizeByFormat(GL_RGBA_GEM);
            pixelformat = k32ARGBPixelFormat;
   } else {
       image_block->setCsizeByFormat(GL_LUMINANCE);
            pixelformat = k8GrayPixelFormat;
   }
#else
   image_block->setCsizeByFormat(GL_RGBA_GEM);
        pixelformat = k32ARGBPixelFormat;
#endif

	::DisposeHandle(reinterpret_cast<Handle>(imageDescH));
   imageDescH = NULL;
  image_block->allocate();

#ifdef __DEBUG__
   post("QTImage2mem() : allocate %d bytes", image_block->xsize*image_block->ysize*image_block->csize);
#endif
        GWorldPtr   gw = NULL;

   OSErr err = QTNewGWorldFromPtr(&gw,
                                 /* taken from pix_filmDarwin */
                                 pixelformat,   // gives noErr
                                 &r, NULL, NULL, 0,
                                 // keepLocal,   
                                 //useDistantHdwrMem, 
                                 image_block->data, 
                                 static_cast<long>(image_block->xsize * image_block->csize));
   if (image_block->data == NULL || err) {
      error("Can't allocate memory for an image.");
   }
   ::GraphicsImportSetGWorld(inImporter, gw, NULL);
   ::GraphicsImportDraw(inImporter);
        ::DisposeGWorld(gw);         //dispose the offscreen
   gw = NULL;

   return image_block;
}
#else
# ifdef HAVE_LIBTIFF
/***************************************************************************
 *
 * Read in a TIFF image.
 *
 ***************************************************************************/
imageStruct *tiffImage2mem(const char *filename)
{
    ::verbose(2, "reading '%s' with libTIFF", filename);
    TIFF *tif = TIFFOpen(filename, "r");
    if (tif == NULL)
    {
       return(NULL);
    }

   uint32 width, height;
   short bits, samps;
   TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
   TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
   TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits);
   TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samps);
    
   int npixels = width * height;

   imageStruct *image_block = new imageStruct;
        
   image_block->xsize = width;
   image_block->ysize = height;
   image_block->type  = GL_UNSIGNED_BYTE;
   image_block->upsidedown = true;

   int knownFormat = 0;
   // Is it a gray8 image?
   if (bits == 8 && samps == 1)
   {
      image_block->csize = samps;
      image_block->format = GL_LUMINANCE;
      knownFormat = 1;
   }
   // Is it an RGB image?
   else if (bits == 8 && samps == 3)
   {
      image_block->csize = 4;
      image_block->format = GL_RGBA;
      knownFormat = 1;
   }
   // Is it an RGBA image?
   else if (bits == 8 && samps == 4)
   {
      image_block->csize = 4;
      image_block->format = GL_RGBA;
      knownFormat = 1;
   }

   // can we handle the raw data?
   if (knownFormat)
   {
       unsigned char *buf = new unsigned char [TIFFScanlineSize(tif)];
       if (buf == NULL)
      {
         error("GemImageLoad(TIFF): can't allocate memory for scanline buffer: %s", filename);
          TIFFClose(tif);
         delete image_block;
         return(NULL);
      }
    
      image_block->allocate(npixels * image_block->csize);
      unsigned char *dstLine = image_block->data;
      int yStride = image_block->xsize * image_block->csize;
      for (uint32 row = 0; row < height; row++)
      {
          unsigned char *pixels = dstLine;
         if (TIFFReadScanline(tif, buf, row, 0) < 0)
         {
             error("GemImageLoad(TIFF): bad image data read on line: %d: %s", row, filename);
             TIFFClose(tif);
            delete image_block;
            delete [] buf;
            return(NULL);
         }
         unsigned char *inp = buf;
         if (samps == 1)
         {
            for (uint32 i = 0; i < width; i++)
            {
                *pixels++ = *inp++;         // Gray8
            }
         }
         else if (samps == 3)
         {
            for (uint32 i = 0; i < width; i++)
            {   
                pixels[chRed]   = inp[0];   // Red
                pixels[chGreen] = inp[1];   // Green
                pixels[chBlue]  = inp[2];   // Blue
                pixels[chAlpha] = 255;      // Alpha
                pixels += 4;
                inp += 3;
            }
         }
         else
         {
            for (uint32 i = 0; i < width; i++)
            {               
                pixels[chRed]   = inp[0];   // Red
                pixels[chGreen] = inp[1];   // Green
                pixels[chBlue]  = inp[2];   // Blue
                pixels[chAlpha] = inp[3];   // Alpha
               pixels += 4;
               inp += 4;
            }
         }
         dstLine += yStride;
      }
      delete [] buf;
   }
   // nope, so use the automatic conversion
   else
   {
      char emsg[1024];
      TIFFRGBAImage img;
      if (TIFFRGBAImageBegin(&img, tif, 0, emsg) == 0)
      {
           //error("GemImageLoad(TIFF): Error reading in image file: %s : %s", filename, emsg);
         delete image_block;
           TIFFClose(tif);
           return(NULL);
      }

      tdata_t raster = _TIFFmalloc(npixels * sizeof(uint32));
      if (raster == NULL)
      {
         error("GemImageLoad(TIFF): Unable to allocate memory for image: %s", filename);
         TIFFClose(tif);
         delete image_block;
         return(NULL);
      }

      if (TIFFRGBAImageGet(&img, raster, width, height) == 0)
      {
         //error("GemImageLoad(TIFF): Error getting image data in file: %s, %s", filename, emsg);
         _TIFFfree(raster);
         TIFFClose(tif);
         delete image_block;
         return(NULL);
      }

       TIFFRGBAImageEnd(&img);

      image_block->csize = 4;
      image_block->format = GL_RGBA;
      image_block->allocate(npixels * image_block->csize);
      unsigned char *dstLine = image_block->data;
      int yStride = image_block->xsize * image_block->csize;
      // transfer everything over
        int k = 0;
        for (uint32 i = 0; i < height; i++)
        {
            unsigned char *pixels = dstLine;
            for (uint32 j = 0; j < width; j++)
            {
	      pixels[chRed]   = static_cast<unsigned char>(TIFFGetR(raster[k])); // Red
	      pixels[chGreen] = static_cast<unsigned char>(TIFFGetG(raster[k])); // Green
	      pixels[chBlue]  = static_cast<unsigned char>(TIFFGetB(raster[k])); // Blue
	      pixels[chAlpha] = static_cast<unsigned char>(TIFFGetA(raster[k])); // Alpha
	      k++;
	      pixels += 4;
            }
            dstLine += yStride;
      }
        _TIFFfree(raster);
   }
   
    TIFFClose(tif);

    return(image_block);
}
# endif /* HAVE_LIBTIFF */
# ifdef HAVE_LIBJPEG
/***************************************************************************
 *
 * Read in a JPEG image.
 *
 * We have to do some funky error handling to keep the jpeg library
 *      from exiting on us.
 *
 ***************************************************************************/

/*****************************
 *
 *   Here is the error handler
 *
 *****************************/
struct my_error_mgr
{
  struct jpeg_error_mgr pub;   // "public" fields
  jmp_buf setjmp_buffer;      // for return to caller
};

typedef struct my_error_mgr * my_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */
METHODDEF(void) my_error_exit (j_common_ptr cinfo)
{
  // cinfo->err really points to a my_error_mgr struct, so coerce pointer
  my_error_ptr myerr = reinterpret_cast<my_error_ptr> (cinfo->err);

  // Always display the message.
  // We could postpone this until after returning, if we chose.
  // (*cinfo->err->output_message) (cinfo);

  // Return control to the setjmp point
  longjmp(myerr->setjmp_buffer, 1);
}

/*
 *   Here is the main image reader
 */
imageStruct *jpegImage2mem(const char *filename)
{
   // open up the file
   FILE * infile;
   ::verbose(2, "reading '%s' with libJPEG", filename);
   if ((infile = fopen(filename, "rb")) == NULL)
   {
       //error("GemImageLoad(JPEG): Unable to open image file: %s", filename);
       return(NULL);
   }

   // create the jpeg structures
   jpeg_decompress_struct cinfo;
   my_error_mgr jerr;

   // We set up the normal JPEG error routines, then override error_exit
   cinfo.err = jpeg_std_error(&jerr.pub);
   jerr.pub.error_exit = my_error_exit;

   // Establish the setjmp return context for my_error_exit to use.
   if ( setjmp(jerr.setjmp_buffer) )
   {
      // If we get here, the JPEG code has signaled an error.
      // We need to clean up the JPEG object, close the input file, and return.
      jpeg_destroy_decompress(&cinfo);
      fclose(infile);
      return(NULL);
   }

   // create the decompression structure
   jpeg_create_decompress(&cinfo);

   // associate the decompress struct with the file
   jpeg_stdio_src(&cinfo, infile);

   // read in the file info
   jpeg_read_header(&cinfo, TRUE);

   imageStruct *image_block = new imageStruct;
   image_block->type  = GL_UNSIGNED_BYTE;

   // do we have an RGB image?
   if (cinfo.jpeg_color_space == JCS_RGB)
   {
      image_block->csize = 4;
      image_block->format = GL_RGBA;
   }
   // do we have a gray8 image?
   else if (cinfo.jpeg_color_space == JCS_GRAYSCALE)
   {
      image_block->csize = 1;
      image_block->format = GL_LUMINANCE;
   }
   // something else, so decompress as RGB
   else
   {
      image_block->csize = 4;
      image_block->format = GL_RGBA;
      cinfo.out_color_space = JCS_RGB;
   }

   // start the decompression
   jpeg_start_decompress(&cinfo);
   int xSize = cinfo.output_width;
   int ySize = cinfo.output_height;
   int cSize = image_block->csize;
   image_block->upsidedown = true;
   image_block->xsize = xSize;
   image_block->ysize = ySize;
   image_block->allocate(xSize * ySize * cSize);
   
   // cycle through the scan lines
   unsigned char *srcLine = new unsigned char[xSize * cSize];
   unsigned char *dstLine = image_block->data;
   int yStride = xSize * cSize;
   int lines = ySize;
   int pixes = xSize;

   // do RGBA/RGB data
   if (cSize == 4)
   {
      while (lines--)
      {
         unsigned char *src = srcLine;
         unsigned char *dst = dstLine;
         jpeg_read_scanlines(&cinfo, &src, 1);
         pixes = xSize;
         while (pixes--)
         {
            dst[chRed] = src[0];
            dst[chGreen] = src[1];
            dst[chBlue] = src[2];
            dst[chAlpha] = 255;
            dst += 4;
            src += 3;
         }
         dstLine += yStride;
      }
   }
   // do grayscale data
   else
   {
      while (lines--)
      {
         unsigned char *src = srcLine;
         unsigned char *dst = dstLine;
         jpeg_read_scanlines(&cinfo, &src, 1);
         pixes = xSize;
         while (pixes--)
         {
            *dst++ = *src++;
         }
         dstLine += yStride;
      }
   }

   // finish the decompression
   jpeg_finish_decompress(&cinfo);
   
   // cleanup
   jpeg_destroy_decompress(&cinfo);
   fclose(infile);
   delete [] srcLine;

   return(image_block);
}
# endif /* HAVE_LIBJPEG */
/***************************************************************************
 *
 * Read in an SGI image.
 *
 ***************************************************************************/
imageStruct *sgiImage2mem(const char *filename)
{
   int32 xsize, ysize, csize;
   if (!sizeofimage(filename, &xsize, &ysize, &csize) )
      return(NULL);

   ::verbose(2, "reading '%s' with SGI", filename);

   imageStruct *image_block = new imageStruct;
   image_block->type  = GL_UNSIGNED_BYTE;
   image_block->xsize = xsize;
   image_block->ysize = ysize;

   if (csize == 4 || csize == 3)
   {
      image_block->csize = 4;
      image_block->format = GL_RGBA;
   }
   else if (csize == 1)
   {
      image_block->csize = 1;
      image_block->format = GL_LUMINANCE;
   }
   else
   {
      //error("GemImageLoad(SGI): unknown color components in SGI file: %s", filename);
      delete image_block;
      return(NULL);
   }
   
   unsigned int32 *readData = longimagedata((char *)filename);
   if (!readData)
   {
      //error("GemImageLoad: error reading SGI image file: %s", filename);
      delete image_block;
      return(NULL);
   }

   image_block->allocate();
   unsigned char *src = reinterpret_cast<unsigned char*>(readData);
   unsigned char *dst = &(image_block->data[0]);
   const int yStride = image_block->xsize * image_block->csize;

   // do RGBA data
   if (csize == 4)
   {
      while (ysize--)
      {
            unsigned char *pixels = dst;
         int count = xsize;
         while(count--)
         {
            pixels[chRed]   = src[0];
            pixels[chGreen] = src[1];
            pixels[chBlue]  = src[2];
            pixels[chAlpha] = src[3];
            pixels += 4;
            src += 4;
         }
            dst += yStride;
      }
   }
   // do RGB data
   else if (csize == 3)
   {
      while (ysize--)
      {
	unsigned char *pixels = dst;
         int count = xsize;
         while(count--)
         {
            pixels[chRed]   = src[0];
            pixels[chGreen] = src[1];
            pixels[chBlue]  = src[2];
            pixels[chAlpha] = 255;;
            pixels += 4;
            src += 4;
         }
            dst += yStride;
      }
   }
   // do grayscale
   else
   {
      while (ysize--)
      {
	unsigned char *pixels = dst;
         int count = xsize;
         while(count--)
         {
            pixels[0] = src[0];
            pixels++;
            src += 4;
         }
            dst += yStride;
      }
   }

   free(readData);
   
   return(image_block);
}
#endif //__APPLE__

#ifdef HAVE_LIBMAGICKPLUSPLUS
imageStruct *magickImage2mem(const char *filename){
  imageStruct *image_block = new imageStruct;
  Magick::Image image;
  try {
    ::verbose(2, "reading '%s' with ImageMagick", filename);
    // Read a file into image object
    image.read( filename );

    image_block->xsize=static_cast<GLint>(image.columns());
    image_block->ysize=static_cast<GLint>(image.rows());
    image_block->setCsizeByFormat(GL_RGBA);
    image_block->reallocate();

    image_block->upsidedown=true;

    image.write(0,0,image_block->xsize,image_block->ysize, 
                "RGBA",
                Magick::CharPixel,
		reinterpret_cast<void*>(image_block->data));
  }catch( Magick::Exception e )  {
    return NULL;
  }
  return image_block;
}

#endif
