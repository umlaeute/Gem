////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
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

namespace
{
static void imageTIFF_verbosehandler(const int verbosity,
                                     const char*module, const char*fmt, va_list ap)
{
  std::string result = "[GEM:imageTIFF] ";
  char buf[MAXPDSTRING];
  if(module) {
    result+=module;
    result+=" ";
  }
  vsnprintf(buf, MAXPDSTRING, fmt, ap);
  buf[MAXPDSTRING-1]=0;
  result+=buf;
  verbose(verbosity, "%s", result.c_str());
}
static void imageTIFF_errorhandler(const char*module, const char*fmt,
                                   va_list ap)
{
  imageTIFF_verbosehandler(-2, module, fmt, ap);
}
static void imageTIFF_warnhandler(const char*module, const char*fmt,
                                  va_list ap)
{
  imageTIFF_verbosehandler(0, module, fmt, ap);
}
typedef struct _imageTIFF_handlers {
  TIFFErrorHandler error, warning;
} t_imageTIFF_handlers;
static t_imageTIFF_handlers imageTIFF_sethandlers(t_imageTIFF_handlers&handlers)
{
  t_imageTIFF_handlers newhandlers;
  newhandlers.error = TIFFSetErrorHandler(handlers.error);
  newhandlers.warning = TIFFSetWarningHandler(handlers.warning);
  return newhandlers;
}
static t_imageTIFF_handlers imageTIFF_sethandlers(void)
{
  t_imageTIFF_handlers handlers;
  handlers.error = TIFFSetErrorHandler(imageTIFF_errorhandler);
  handlers.warning = TIFFSetWarningHandler(imageTIFF_warnhandler);
  return handlers;
}
#define tiffhandlers_init()   t_imageTIFF_handlers tiffhandler = imageTIFF_sethandlers()
#define tiffhandlers_cleanup() imageTIFF_sethandlers(tiffhandler)


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
  static bool firsttime=true;
  if(firsttime) {
    TIFFSetErrorHandler(imageTIFF_errorhandler);
    TIFFSetWarningHandler(imageTIFF_warnhandler);
  }
  firsttime=false;
}
imageTIFF :: ~imageTIFF(void)
{
}

/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
bool imageTIFF :: load(std::string filename, imageStruct&result,
                       gem::Properties&props)
{
  tiffhandlers_init();
  TIFF *tif = TIFFOpen(filename.c_str(), "r");
  if (tif == NULL) {
    tiffhandlers_cleanup();
    return false;
  }

  uint32_t width, height;
  uint16_t orientation;
  short bits, samps;
  TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
  TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits);
  TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samps);
  TIFFGetField(tif, TIFFTAG_ORIENTATION, &orientation);

  int npixels = width * height;

  result.xsize=width;
  result.ysize=height;
  result.upsidedown=(ORIENTATION_BOTLEFT != orientation);

  bool knownFormat = false;
  // Is it a gray8 image?
  if (bits == 8 && samps == 1) {
    result.setFormat(GEM_GRAY);
    knownFormat = true;
  }
  // Is it an RGB image?
  else if (bits == 8 && samps == 3) {
    result.setFormat(GEM_RGBA);
    knownFormat = true;
  }
  // Is it an RGBA image?
  else if (bits == 8 && samps == 4) {
    result.setFormat(GEM_RGBA);
    knownFormat = true;
  }

  // can we handle the raw data?
  if (knownFormat) {
    unsigned char *buf = new unsigned char [TIFFScanlineSize(tif)];
    if (buf == NULL) {
      pd_error(0, "[GEM:imageTIFF] can't allocate memory for scanline buffer: %s",
               filename.c_str());
      TIFFClose(tif);
      tiffhandlers_cleanup();
      return false;
    }

    result.reallocate();
    unsigned char *dstLine = result.data;
    int yStride = result.xsize * result.csize;
    for (uint32_t row = 0; row < height; row++) {
      unsigned char *pixels = dstLine;
      if (TIFFReadScanline(tif, buf, row, 0) < 0) {
        verbose(1, "[GEM:imageTIFF] bad image data read on line: %d: %s", row,
                filename.c_str());
        TIFFClose(tif);
        return false;
      }
      unsigned char *inp = buf;
      if (samps == 1) {
        for (uint32_t i = 0; i < width; i++) {
          *pixels++ = *inp++;         // Gray8
        }
      } else if (samps == 3)  {
        for (uint32_t i = 0; i < width; i++) {
          pixels[chRed]   = inp[0];   // Red
          pixels[chGreen] = inp[1];   // Green
          pixels[chBlue]  = inp[2];   // Blue
          pixels[chAlpha] = 255;      // Alpha
          pixels += 4;
          inp += 3;
        }
      } else {
        for (uint32_t i = 0; i < width; i++) {
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
      verbose(0, "[GEM:imageTIFF] Error reading in image file '%s': %s",
              filename.c_str(), emsg);
      TIFFClose(tif);
      tiffhandlers_cleanup();
      return false;
    }

    uint32_t*raster = reinterpret_cast<uint32_t*>(_TIFFmalloc(npixels * sizeof(
                      uint32_t)));
    if (raster == NULL) {
      pd_error(0, "[GEM:imageTIFF] Unable to allocate memory for image '%s'",
               filename.c_str());
      TIFFClose(tif);
      tiffhandlers_cleanup();
      return false;
    }

    if (TIFFRGBAImageGet(&img, raster, width, height) == 0) {
      verbose(0, "[GEM:imageTIFF] Error getting image data in file '%s': %s",
              filename.c_str(), emsg);
      _TIFFfree(raster);
      TIFFClose(tif);
      tiffhandlers_cleanup();
      return false;
    }

    TIFFRGBAImageEnd(&img);
    result.setFormat(GEM_RGBA);
    result.reallocate();

    unsigned char *dstLine = result.data;
    int yStride = result.xsize * result.csize;
    // transfer everything over
    int k = 0;
    for (uint32_t i = 0; i < height; i++) {
      unsigned char *pixels = dstLine;
      for (uint32_t j = 0; j < width; j++) {
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

  result.fixUpDown();

  double value_d;
  short value_i16;
  char value_s[MAXPDSTRING];
  if(TIFFGetField(tif, TIFFTAG_XRESOLUTION, &value_d)) {
    props.set("xresolution", value_d);
  }
  if(TIFFGetField(tif, TIFFTAG_YRESOLUTION, &value_d)) {
    props.set("yresolution", value_d);
  }
  if(TIFFGetField(tif, TIFFTAG_XRESOLUTION, &value_d)) {
    props.set("xresolution", value_d);
  }
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
  if(TIFFGetField(tif, TIFFTAG_SOFTWARE, &value_s)) {
    props.set("software", std::string(value_s));
  }
  if(TIFFGetField(tif, TIFFTAG_ARTIST, &value_s)) {
    props.set("artist", std::string(value_s));
  }
  if(TIFFGetField(tif, TIFFTAG_HOSTCOMPUTER, &value_s)) {
    props.set("hostcomputer", std::string(value_s));
  }

  TIFFClose(tif);

  const char*orient=0;
  switch(orientation) {
  case ORIENTATION_TOPLEFT:
  case ORIENTATION_BOTLEFT:
    break;
  case ORIENTATION_TOPRIGHT:
    orient = "top right"; break;
  case ORIENTATION_BOTRIGHT:
    orient = "bottom right"; break;
  case ORIENTATION_LEFTTOP:
    orient = "left top"; break;
  case ORIENTATION_RIGHTTOP:
    orient = "right top"; break;
  case ORIENTATION_RIGHTBOT:
    orient = "right bottom"; break;
  case ORIENTATION_LEFTBOT:
    orient = "left bottom"; break;
  default:
    orient = "unknown"; break;
  }
  if(orient) {
    verbose(0, "[GEM:imageTIFF] unhandled orientation '%s' (%d)", orient, orientation);
  }
  return true;
}
bool imageTIFF::save(const imageStruct&constimage,
                     const std::string&filename, const std::string&mimetype,
                     const gem::Properties&props)
{
  tiffhandlers_init();
  TIFF *tif = NULL;
  imageStruct image;

  tif=TIFFOpen(filename.c_str(), "w");
  if (tif == NULL) {
      tiffhandlers_cleanup();
      return false;
  }
  image.convertFrom(&constimage, GEM_RAW_RGBA);
  image.fixUpDown();

  uint32_t width=image.xsize, height = image.ysize;
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
    if(("inch"==resunit_s) || ("english"==resunit_s)
        || ("imperial"==resunit_s)) {
      resunit=RESUNIT_INCH;
    } else if(("centimeter"==resunit_s) || ("metric"==resunit_s)) {
      resunit=RESUNIT_CENTIMETER;
    } else {
      resunit=RESUNIT_NONE;
    }
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

  uint16_t extra_samples[1] = { EXTRASAMPLE_UNASSALPHA };
  TIFFSetField(tif, TIFFTAG_EXTRASAMPLES, 1, extra_samples);

  if(!software.empty()) {
    TIFFSetField(tif, TIFFTAG_SOFTWARE, software.c_str());
  }
  if(!artist.empty()) {
    TIFFSetField(tif, TIFFTAG_ARTIST, artist.c_str());
  }
  if(!hostcomputer.empty()) {
    TIFFSetField(tif, TIFFTAG_HOSTCOMPUTER, hostcomputer.c_str());
  }

  int yStride = image.xsize * image.csize;
  unsigned char *srcLine = image.data;

  for (uint32_t row = 0; row < height; row++) {
    if (TIFFWriteScanline(tif, srcLine, row, 0) < 0) {
      verbose(0, "[GEM:imageTIFF] could not write line %d to image '%s'", row,
              filename.c_str());
      TIFFClose(tif);
      tiffhandlers_cleanup();
      return false;
    }
    srcLine += yStride;
  }
  TIFFClose(tif);

  tiffhandlers_cleanup();
  return true;
}


float imageTIFF::estimateSave(const imageStruct&img,
                              const std::string&filename, const std::string&mimetype,
                              const gem::Properties&props)
{
  float result=0;
  if(mimetype == "image/tiff" || mimetype == "image/x-tiff") {
    result += 100;
  }

  if(gem::Properties::UNSET != props.type("xresolution")) {
    result+=1.;
  }
  if(gem::Properties::UNSET != props.type("yresolution")) {
    result+=1.;
  }
  if(gem::Properties::UNSET != props.type("resolutionunit")) {
    result+=1.;
  }
  if(gem::Properties::UNSET != props.type("software")) {
    result+=1.;
  }
  if(gem::Properties::UNSET != props.type("artist")) {
    result+=1.;
  }
  if(gem::Properties::UNSET != props.type("hostcomputer")) {
    result+=1.;
  }

  return result;
}


void imageTIFF::getWriteCapabilities(std::vector<std::string>&mimetypes,
                                     gem::Properties&props)
{
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
