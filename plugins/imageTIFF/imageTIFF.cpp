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

#ifdef _WIN32
#include <stdio.h> // AV : in windows, this should be included before tiffio.h
#endif /* WINVER */

#ifdef HAVE_LIBTIFF
extern "C"
{
# include "tiffio.h"
}

#include <string.h>
#include "imageTIFF.h"
#include "plugins/PluginFactory.h"

#include "Gem/RTE.h"


using namespace gem::plugins;

REGISTER_IMAGELOADERFACTORY("tiff", imageTIFF);
REGISTER_IMAGESAVERFACTORY("tiff", imageTIFF);


#ifdef _MSC_VER
# define vsnprintf _vsnprintf
#endif

namespace {
 static void imageTIFF_verbosehandler(const int verbosity, const char*module, const char*fmt, va_list ap) {
   std::string result=module;
   result+=": ";
   char buf[MAXPDSTRING];
   vsnprintf(buf, MAXPDSTRING, fmt, ap);
   buf[MAXPDSTRING-1]=0;
   result+=buf;
   verbose(verbosity, "%s", result.c_str());
 }
 static void imageTIFF_errorhandler(const char*module, const char*fmt, va_list ap) {
   imageTIFF_verbosehandler(2, module, fmt, ap);
 }
 static void imageTIFF_warnhandler(const char*module, const char*fmt, va_list ap) {
   imageTIFF_verbosehandler(3, module, fmt, ap);
 }
};

/////////////////////////////////////////////////////////
//
// imageTIFF
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

imageTIFF :: imageTIFF(void)
{
  //post("imageTIFF");
 bool firsttime=true;
 if(firsttime) {
   TIFFSetErrorHandler(imageTIFF_errorhandler);
   TIFFSetWarningHandler(imageTIFF_warnhandler);
 }
 firsttime=false;
}
imageTIFF :: ~imageTIFF(void)
{
  //post("~imageTIFF");
}

/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
bool imageTIFF :: load(std::string filename, imageStruct&result, gem::Properties&props)
{
  ::verbose(2, "reading '%s' with libTIFF", filename.c_str());
  TIFF *tif = TIFFOpen(filename.c_str(), "r");
  if (tif == NULL) {
      return(NULL);
    }

  uint32 width, height;
  short bits, samps;
  TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
  TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits);
  TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samps);

  int npixels = width * height;

  result.xsize=width;
  result.ysize=height;
  result.upsidedown=true;
  result.type=GL_UNSIGNED_BYTE; //?

  bool knownFormat = false;
  // Is it a gray8 image?
  if (bits == 8 && samps == 1)
    {
      result.setCsizeByFormat(GL_LUMINANCE);
      knownFormat = true;
    }
  // Is it an RGB image?
  else if (bits == 8 && samps == 3)
    {
      result.setCsizeByFormat(GL_RGBA);
      knownFormat = true;
    }
  // Is it an RGBA image?
  else if (bits == 8 && samps == 4)
    {
      result.setCsizeByFormat(GL_RGBA);
      knownFormat = true;
    }

  // can we handle the raw data?
  if (knownFormat) {
    unsigned char *buf = new unsigned char [TIFFScanlineSize(tif)];
    if (buf == NULL) {
      error("GemImageLoad(TIFF): can't allocate memory for scanline buffer: %s", filename.c_str());
      TIFFClose(tif);
      return(false);
    }

    result.reallocate();
    unsigned char *dstLine = result.data;
    int yStride = result.xsize * result.csize;
    for (uint32 row = 0; row < height; row++)
      {
	unsigned char *pixels = dstLine;
	if (TIFFReadScanline(tif, buf, row, 0) < 0) {
	  error("GemImageLoad(TIFF): bad image data read on line: %d: %s", row, filename.c_str());
	  TIFFClose(tif);
	  return false;
	}
	unsigned char *inp = buf;
	if (samps == 1) {
	  for (uint32 i = 0; i < width; i++) {
	    *pixels++ = *inp++;         // Gray8
	  }
	}
	else if (samps == 3)  {
	  for (uint32 i = 0; i < width; i++) {
	    pixels[chRed]   = inp[0];   // Red
	    pixels[chGreen] = inp[1];   // Green
	    pixels[chBlue]  = inp[2];   // Blue
	    pixels[chAlpha] = 255;      // Alpha
	    pixels += 4;
	    inp += 3;
	  }
	} else {
	  for (uint32 i = 0; i < width; i++) {
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
  else {
    char emsg[1024];
    TIFFRGBAImage img;
    if (TIFFRGBAImageBegin(&img, tif, 0, emsg) == 0) {
      //error("GemImageLoad(TIFF): Error reading in image file: %s : %s", filename, emsg);
      TIFFClose(tif);
      return(false);
    }

    uint32*raster = reinterpret_cast<uint32*>(_TIFFmalloc(npixels * sizeof(uint32)));
    if (raster == NULL) {
      error("GemImageLoad(TIFF): Unable to allocate memory for image: %s", filename.c_str());
      TIFFClose(tif);
      return(false);
    }

    if (TIFFRGBAImageGet(&img, raster, width, height) == 0) {
      //error("GemImageLoad(TIFF): Error getting image data in file: %s, %s", filename, emsg);
      _TIFFfree(raster);
      TIFFClose(tif);
      return(false);
    }

    TIFFRGBAImageEnd(&img);
    result.setCsizeByFormat(GL_RGBA);
    result.reallocate();

    unsigned char *dstLine = result.data;
    int yStride = result.xsize * result.csize;
    // transfer everything over
    int k = 0;
    for (uint32 i = 0; i < height; i++) {
      unsigned char *pixels = dstLine;
      for (uint32 j = 0; j < width; j++) {
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


  double value_d;
  short value_i16;
  char value_s[MAXPDSTRING];
  if(TIFFGetField(tif, TIFFTAG_XRESOLUTION, &value_d)) props.set("xresolution", value_d);
  if(TIFFGetField(tif, TIFFTAG_YRESOLUTION, &value_d)) props.set("yresolution", value_d);
  if(TIFFGetField(tif, TIFFTAG_XRESOLUTION, &value_d)) props.set("xresolution", value_d);
  if(TIFFGetField(tif, TIFFTAG_RESOLUTIONUNIT, &value_i16)) {
    std::string resunit;
    switch(value_i16) {
    case RESUNIT_INCH:
      resunit="inch";
      break;
    case RESUNIT_CENTIMETER:
      resunit="centimeter";
      break;
    default:
      resunit="none";
      break;
    }
    props.set("resolutionunit", resunit);
  }
  if(TIFFGetField(tif, TIFFTAG_SOFTWARE, &value_s)) props.set("software", std::string(value_s));
  if(TIFFGetField(tif, TIFFTAG_ARTIST, &value_s)) props.set("artist", std::string(value_s));
  if(TIFFGetField(tif, TIFFTAG_HOSTCOMPUTER, &value_s)) props.set("hostcomputer", std::string(value_s));

  TIFFClose(tif);
  return true;
}
bool imageTIFF::save(const imageStruct&constimage, const std::string&filename, const std::string&mimetype, const gem::Properties&props) {
  TIFF *tif = NULL;

  if(GL_YUV422_GEM==constimage.format) {
    error("don't know how to write YUV-images with libTIFF");
    return false;
  }

  tif=TIFFOpen(filename.c_str(), "w");
  if (tif == NULL) {
    return false;
  }
  imageStruct image; constimage.copy2Image(&image);
  image.fixUpDown();

  uint32 width=image.xsize, height = image.ysize;
  short bits=8, samps=image.csize;
  int npixels = width * height;
  //int planar_conf = PLANARCONFIG_CONTIG;
  std::string software = "PD/GEM";
  std::string artist;
  std::string hostcomputer;

  double xresolution = 72., yresolution=72.;
  short resunit = RESUNIT_INCH;

  props.get("xresolution", xresolution);
  props.get("yresolution", yresolution);
  std::string resunit_s;
  if(props.get("resolutionunit", resunit_s)) {
    if(("inch"==resunit_s) || ("english"==resunit_s) || ("imperial"==resunit_s))
      resunit=RESUNIT_INCH;
    else if(("centimeter"==resunit_s) || ("metric"==resunit_s))
      resunit=RESUNIT_CENTIMETER;
    else
      resunit=RESUNIT_NONE;
  }
  props.get("software", software);
  props.get("artist", artist);
  props.get("hostcomputer", hostcomputer);


  TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
  TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
  TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bits);
  TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samps);
  TIFFSetField(tif, TIFFTAG_PLANARCONFIG, 1);
  TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

  TIFFSetField(tif, TIFFTAG_XRESOLUTION, xresolution); // RATIONAL
  TIFFSetField(tif, TIFFTAG_YRESOLUTION, yresolution); // RATIONAL
  TIFFSetField(tif, TIFFTAG_RESOLUTIONUNIT, resunit);

  if(!software.empty())
    TIFFSetField(tif, TIFFTAG_SOFTWARE, software.c_str());
  if(!artist.empty())
    TIFFSetField(tif, TIFFTAG_ARTIST, artist.c_str());
  if(!hostcomputer.empty())
    TIFFSetField(tif, TIFFTAG_HOSTCOMPUTER, hostcomputer.c_str());

  int yStride = image.xsize * image.csize;
  unsigned char *srcLine = &(image.data[npixels * image.csize]);
  srcLine -= yStride;

  for (uint32 row = 0; row < height; row++) {
    unsigned char *buf = srcLine;
    if (TIFFWriteScanline(tif, buf, row, 0) < 0)
      {
        error("GEM: could not write line %d to image %s", row, filename.c_str());
        TIFFClose(tif);
        delete [] buf;
        return(false);
      }
    srcLine -= yStride;
  }
  TIFFClose(tif);

  return true;
}


float imageTIFF::estimateSave(const imageStruct&img, const std::string&filename, const std::string&mimetype, const gem::Properties&props) {
  float result=0;
  if(mimetype == "image/tiff" || mimetype == "image/x-tiff")
    result += 100;

  if(gem::Properties::UNSET != props.type("xresolution"))result+=1.;
  if(gem::Properties::UNSET != props.type("yresolution"))result+=1.;
  if(gem::Properties::UNSET != props.type("resolutionunit"))result+=1.;
  if(gem::Properties::UNSET != props.type("software"))result+=1.;
  if(gem::Properties::UNSET != props.type("artist"))result+=1.;
  if(gem::Properties::UNSET != props.type("hostcomputer"))result+=1.;

  return result;
}


void imageTIFF::getWriteCapabilities(std::vector<std::string>&mimetypes, gem::Properties&props) {
  mimetypes.clear();
  props.clear();

  mimetypes.push_back("image/tiff");
  mimetypes.push_back("image/x-tiff");

  gem::any value;


  value=72.f;
  props.set("xresolution", value);
  props.set("yresolution", value);

  value=std::string("inch");
  props.set("resolutionunit", value);
  value=std::string("PD/GEM");
  props.set("software", value);
  value=std::string("");
  props.set("artist", value);
  props.set("hostcomputer", value);
}
#endif
