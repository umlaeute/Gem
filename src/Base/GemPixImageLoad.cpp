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

#include "GemPixImageLoad.h"

#include "m_pd.h"

#ifdef MACOSX
#include <Carbon/carbon.h>
#include <QuickTime/QuickTime.h>
#include <QuickTime/ImageCompression.h>
#include <QuickTime/Movies.h>
#include <QuickTime/QuickTimeComponents.h>
#include <string.h>
#endif // MACOSX

#ifdef _WINDOWS
#include <io.h>
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#include <string.h>

#ifndef MACOSX
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
#endif // MACOSX

#include "GemPixUtil.h"

#ifdef MACOSX
imageStruct *QTImage2mem(GraphicsImportComponent inImporter);
OSStatus FSPathMakeFSSpec(
	const UInt8 *path,
	FSSpec *spec,
	Boolean *isDirectory);
#else
imageStruct *tiffImage2mem(const char *filename);
imageStruct *jpegImage2mem(const char *filename);
imageStruct *sgiImage2mem(const char *filename);


/* this function gives us 32 byte aligned data, the cache line size
   of  pentiums */

unsigned char* img_allocate(int size)
{
  unsigned char* pad;
  unsigned char* data;

#if 0
  pad = new unsigned char[size];
  data = (unsigned char*) ((((unsigned int)pad)+31)& (~31));
#else
  data = new unsigned char [size];
#endif

  return data; 
}
#endif // MACOSX

/***************************************************************************
 *
 * image2mem - Read in an image in various file formats
 *
 ***************************************************************************/
#ifdef MACOSX
GEM_EXTERN imageStruct *image2mem(const char *filename)
{
	OSErr				err;
	imageStruct 			*image_block = NULL;
	GraphicsImportComponent 	importer = NULL;

	// does the file even exist?
	if (filename[0] != '\0') {
		FSSpec	spec;

		err = ::FSPathMakeFSSpec( (Str255)filename, &spec, NULL);
		if (err) {
			error("GEM: Unable to find file: %#s", spec.name);
                        error("GEM: Unable to find filename:%s", filename);
			error("parID : %d", spec.parID); 
			return NULL;
		}
		err = ::GetGraphicsImporterForFile(&spec, &importer);
		if (err) {
			error("GEM: Unable to import an image: %#s", spec.name);
			return NULL;
		}
	}
	image_block = QTImage2mem(importer);
	::CloseComponent(importer);
	if (image_block)	return image_block;
	else			return NULL;
}
/*****************************************************************************/

OSStatus
FSPathMakeFSSpec(
	const UInt8 *path,
	FSSpec *spec,
	Boolean *isDirectory)	/* can be NULL */
{
	OSStatus	result;
	FSRef		ref;
	
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

	// does the file even exist?
	if (filename[0] == '/' || filename[0] == '\\')
	{
		strcpy(newName, filename);
	}
	else
	{
		char realName[256];
		char *realResult;

		int fd = open_via_path(".", filename, "", realName, &realResult, 256, 0);
		if (fd < 0)
		{
			error("GEM: Unable to find file: %s", filename);
			return(NULL);
		}
		else
		{
			int res = close(fd);
			if (res == -1)
			{
				int foo;
				foo = 1;
			}
		}

		sprintf(newName, "%s/%s", realName, realResult);
	}

	
	// try to load in a JPEG file
	if ( (image_block = jpegImage2mem(newName)) )
			return(image_block);

	// try to load in an SGI file
	if ( (image_block = sgiImage2mem(newName)) )
			return(image_block);

	// try to load in a TIFF file
	if ( (image_block = tiffImage2mem(newName)) )
			return(image_block);

	// unable to load image
	error("GEM: Unable to load image: %s", newName);
	return(NULL);
}
#endif // MACOSX
/***************************************************************************
 *
 * Read in a image utilizing QuickTime GraphicsImporterComponent
 *
 ***************************************************************************/
#ifdef MACOSX
imageStruct *QTImage2mem(GraphicsImportComponent inImporter)
{
	Rect		r;
	if (::GraphicsImportGetNaturalBounds(inImporter, &r)) return NULL;	//get an image size
	::OffsetRect(&r, -r.left, -r.top);									
	if (::GraphicsImportSetBoundsRect(inImporter, &r)) return NULL;		
	ImageDescriptionHandle imageDescH = NULL;
	if (::GraphicsImportGetImageDescription(inImporter, &imageDescH)) return NULL;
		
	imageStruct *image_block = new imageStruct;
	image_block->xsize	= (*imageDescH)->width;
	image_block->ysize	= (*imageDescH)->height;
	//image_block->type	= GL_UNSIGNED_BYTE;
        //image_block->type	= GL_UNSIGNED_INT_8_8_8_8_REV;
        image_block->type	= GL_UNSIGNED_INT_8_8_8_8;
	if ((*imageDescH)->depth <= 32) {
		image_block->csize = 4;
		//image_block->format = GL_RGBA;
                image_block->format = GL_BGRA_EXT;
	} else {
		image_block->csize = 1;
		image_block->format = GL_LUMINANCE;
	}
	::DisposeHandle((Handle)imageDescH);
	imageDescH = NULL;
	const int dataSize = image_block->ysize * image_block->xsize * image_block->csize;

	image_block->data = new unsigned char[dataSize];
#ifdef __DEBUG__
	post("QTImage2mem() : allocate %d bytes", dataSize);
#endif
        GWorldPtr	gw = NULL;
/*
#ifdef __QTNEWGWORLDFROMPTR__
	OSErr err = QTNewGWorldFromPtr(&gw,  
                                    //k32RGBAPixelFormat,
                                    k32ARGBPixelFormat,
                                    &r, NULL, NULL, 
                                    keepLocal,	
                                    //useDistantHdwrMem, 
                                    image_block->data, 
                                    (long)(image_block->xsize * image_block->csize));
	if (image_block->data == NULL || err) {
		error("Can't allocate memory for an image.");
	}
	//image_block->pixelFormat = k32RGBAPixelFormat;
        //image_block->pixelFormat = k32ARGBPixelFormat;
	//image_block->pixMapH = GetGWorldPixMap(image_block->gw);
	//::LockPixels(image_block->pixMapH);
	::GraphicsImportSetGWorld(inImporter, gw, NULL);
	::GraphicsImportDraw(inImporter);
        ::DisposeGWorld(gw);			//dispose the offscreen
	gw = NULL;
        
#else	//don't "USE_QTNEWGWORLDFROMPTR"
*/	::NewGWorld(&gw, k32ARGBPixelFormat, &r, NULL, NULL, keepLocal);	//make an offscreen
	::PixMapHandle pixmapH = GetGWorldPixMap(gw);
        //image_block->pixelFormat = k32ARGBPixelFormat;
	::LockPixels(pixmapH);						//lock a pixmap

	::GraphicsImportSetGWorld(inImporter, gw, NULL);		//set graphics port to draw an image
	::GraphicsImportDraw(inImporter);				//draw an image to offscreen

	unsigned char *baseP = (unsigned char *)::GetPixBaseAddr(pixmapH);	//get base address of pixels
	unsigned long rowbytes = (unsigned long)::GetPixRowBytes(pixmapH);	//get the number of bytes per line

	if (image_block->csize == 4) {
		for (int i=0, y=image_block->ysize-1; y>=0; y--) {
			unsigned char *tmpP = baseP + y * rowbytes;
			for (int x=image_block->xsize-1; x>=0; x--) {
				/*image_block->data[i++] = *(tmpP + 1);
				image_block->data[i++] = *(tmpP + 2);
				image_block->data[i++] = *(tmpP + 3);
				image_block->data[i++] = *(tmpP);*/
                                image_block->data[i++] = *(tmpP + 3);
				image_block->data[i++] = *(tmpP + 2);
				image_block->data[i++] = *(tmpP + 1);
				image_block->data[i++] = *(tmpP);
				tmpP += 4;
			}
		}
	} else if (image_block->csize == 1) {
		for (int i=0, y=image_block->ysize-1; y>=0; y--) {
			unsigned char *tmpP = baseP + y * rowbytes;
			for (int x=image_block->xsize-1; x>=0; x--) {
				image_block->data[i++] = 255 - *(tmpP);
			}
			tmpP += 1;
		}
	} else {
	}

	::UnlockPixels(pixmapH);		//unlock the pixmap
	pixmapH = NULL;				//
	::DisposeGWorld(gw);			//dispose the offscreen
	gw = NULL;
	return image_block;
//#endif	//__QTNEWGWORLDFROMPTR__
}
#else
/***************************************************************************
 *
 * Read in a TIFF image.
 *
 ***************************************************************************/
imageStruct *tiffImage2mem(const char *filename)
{
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
			error("GEM: can't allocate memory for scanline buffer: %s", filename);
		    TIFFClose(tif);
			delete image_block;
			return(NULL);
		}
    
		image_block->allocate(npixels * image_block->csize);
		unsigned char *dstLine = &(image_block->data[npixels * image_block->csize]);
        int yStride = image_block->xsize * image_block->csize;
        dstLine -= yStride;
	    for (uint32 row = 0; row < height; row++)
		{
    		unsigned char *pixels = dstLine;
			if (TIFFReadScanline(tif, buf, row, 0) < 0)
			{
			    error("GEM: bad image data read on line: %d: %s", row, filename);
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
    				*pixels++ = *inp++;			// Gray8
				}
			}
			else if (samps == 3)
			{
				for (uint32 i = 0; i < width; i++)
				{	
    				pixels[chRed]   = inp[0];	// Red
    				pixels[chGreen] = inp[1];	// Green
    				pixels[chBlue]  = inp[2];	// Blue
    				pixels[chAlpha] = 255;		// Alpha
					pixels += 4;
					inp += 3;
				}
			}
			else
			{
				for (uint32 i = 0; i < width; i++)
				{					
    				pixels[chRed]   = inp[0];	// Red
    				pixels[chGreen] = inp[1];	// Green
    				pixels[chBlue]  = inp[2];	// Blue
    				pixels[chAlpha] = inp[3];	// Alpha
					pixels += 4;
					inp += 4;
				}
			}
            dstLine -= yStride;
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
	        error("GEM: Error reading in image file: %s : %s", filename, emsg);
			delete image_block;
	        TIFFClose(tif);
	        return(NULL);
		}

		uint32 *raster = (uint32 *) _TIFFmalloc(npixels * sizeof(uint32));
		if (raster == NULL)
		{
			error("GEM: Unable to allocate memory for image: %s", filename);
			TIFFClose(tif);
			delete image_block;
			return(NULL);
		}

		if (TIFFRGBAImageGet(&img, raster, width, height) == 0)
		{
			error("GEM: Error getting image data in file: %s, %s", filename, emsg);
			_TIFFfree(raster);
			TIFFClose(tif);
			delete image_block;
			return(NULL);
		}

	    TIFFRGBAImageEnd(&img);

		image_block->csize = 4;
		image_block->format = GL_RGBA;
		image_block->allocate(npixels * image_block->csize);
		unsigned char *dstLine = &(image_block->data[npixels * image_block->csize]);
        int yStride = image_block->xsize * image_block->csize;
        dstLine -= yStride;
		// transfer everything over
        int k = 0;
        for (uint32 i = 0; i < height; i++)
        {
            unsigned char *pixels = dstLine;
            for (uint32 j = 0; j < width; j++)
            {
			    pixels[chRed]   = (unsigned char)TIFFGetR(raster[k]); // Red
			    pixels[chGreen] = (unsigned char)TIFFGetG(raster[k]); // Green
			    pixels[chBlue]  = (unsigned char)TIFFGetB(raster[k]); // Blue
			    pixels[chAlpha] = (unsigned char)TIFFGetA(raster[k]); // Alpha
                k++;
                pixels += 4;
            }
            dstLine -= yStride;
		}
        _TIFFfree(raster);
	}
	
    TIFFClose(tif);

    return(image_block);
}

/***************************************************************************
 *
 * Read in a JPEG image.
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
  jmp_buf setjmp_buffer;		// for return to caller
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
 *	Here is the main image reader
 */
imageStruct *jpegImage2mem(const char *filename)
{
	// open up the file
	FILE * infile;
	if ((infile = fopen(filename, "rb")) == NULL)
	{
	    error("GEM: Unable to open image file: %s", filename);
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
	image_block->xsize = xSize;
	image_block->ysize = ySize;
	image_block->allocate(xSize * ySize * cSize);
	
	// cycle through the scan lines
	unsigned char *srcLine = new unsigned char[xSize * cSize];
	unsigned char *dstLine = &(image_block->data[xSize * ySize * cSize]);
	int yStride = xSize * cSize;
    dstLine -= yStride;
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
			dstLine -= yStride;
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
			dstLine -= yStride;
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

/***************************************************************************
 *
 * Read in an SGI image.
 *
 ***************************************************************************/
imageStruct *sgiImage2mem(const char *filename)
{
	int32 xsize, ysize, csize;

	if (!sizeofimage((char *)filename, &xsize, &ysize, &csize) )
		return(NULL);

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
		error("GEM: unknown color components in SGI file: %s", filename);
		delete image_block;
		return(NULL);
	}
	
	unsigned int32 *readData = longimagedata((char *)filename);
	if (!readData)
	{
		error("GEM: error reading SGI image file: %s", filename);
		delete image_block;
		return(NULL);
	}

	const int dataSize = image_block->ysize * image_block->xsize * image_block->csize;
	image_block->allocate(dataSize);
	unsigned char *src = (unsigned char *)readData;
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
#endif //MACOSX

