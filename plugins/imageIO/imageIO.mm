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

#include <string.h>
#include "imageIO.h"
#include "plugins/PluginFactory.h"

#include "Gem/RTE.h"
#import <Foundation/Foundation.h>
#import <ImageIO/ImageIO.h>

using namespace gem::plugins;

REGISTER_IMAGELOADERFACTORY("imageIO", imageIO);
REGISTER_IMAGESAVERFACTORY("imageIO", imageIO);

CFStringRef mime2uti(const std::string&mimetype)
{
  if(mimetype == "image/jpeg") {
    return kUTTypeJPEG;
  } else if ((mimetype == "image/jp2") || (mimetype == "image/jpx") || (mimetype == "image/jpm")) {
    return kUTTypeJPEG2000;
  } else if ((mimetype == "image/tiff") || (mimetype == "image/x-tiff")) {
    return kUTTypeTIFF;
  } else if ((mimetype == "image/pict") || (mimetype == "image/x-pict")) {
    return kUTTypePICT;
  } else if ((mimetype == "image/gif")) {
    return kUTTypeGIF;
  } else if ((mimetype == "image/png")) {
    return kUTTypePNG;
  } else if ((mimetype == "image/x-quicktime")) {
    return kUTTypeQuickTimeImage;
  } else if ((mimetype == "image/icns")) {
    return kUTTypeAppleICNS;
  } else if ((mimetype == "image/bmp") || (mimetype == "image/x-windows-bmp")) {
    return kUTTypeBMP;
  } else if ((mimetype == "image/x-icon")) {
    return kUTTypeICO;
  }

  return 0;
}


/////////////////////////////////////////////////////////
//
// imageIO
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

imageIO :: imageIO(void)
{
}
imageIO :: ~imageIO(void)
{
}

/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
bool imageIO :: load(std::string filename, imageStruct&result,
                     gem::Properties&props)
{
  bool success = false;
  NSString *path = [NSString stringWithUTF8String:filename.c_str()];
  // Get the URL for the pathname passed to the function.
  NSURL *url = [NSURL fileURLWithPath:path];
  CGImageRef myImage = NULL;
  CGImageSourceRef myImageSource;
  CFDictionaryRef myOptions = NULL;
  CFStringRef myKeys[2];
  CFTypeRef myValues[2];

  // Set up options if you want them. The options here are for
  // caching the image in a decoded form and for using floating-point
  // values if the image format supports them.
  myKeys[0] = kCGImageSourceShouldCache;
  myValues[0] = (CFTypeRef)kCFBooleanTrue;
  myKeys[1] = kCGImageSourceShouldAllowFloat;
  myValues[1] = (CFTypeRef)kCFBooleanTrue;

  // Create the dictionary
  myOptions = CFDictionaryCreate(NULL, (const void **) myKeys,
                                 (const void **) myValues, 2,
                                 &kCFTypeDictionaryKeyCallBacks,
                                 & kCFTypeDictionaryValueCallBacks);
  // Create an image source from the URL.
  myImageSource = CGImageSourceCreateWithURL((CFURLRef)url, myOptions);
  CFRelease(myOptions);
  // Make sure the image source exists before continuing
  if (!myImageSource) {
    fprintf(stderr, "Image source '%s' is NULL.", filename.c_str());
    return false;
  }

  // Create an image from the first item in the image source.
  myImage = CGImageSourceCreateImageAtIndex(myImageSource,
            0,
            NULL);

  // get the orientation
  int fixUpDown = 0;
  CFDictionaryRef dict = CGImageSourceCopyPropertiesAtIndex(myImageSource, 0, NULL);
  if(dict) {
    //CFShow(dict);
    NSDictionary*myProps = CFBridgingRelease(dict);
    int orientation = [[myProps objectForKey:@"Orientation"] integerValue];
    switch (orientation) {
      default:
        fixUpDown = 0;
        break;
      case kCGImagePropertyOrientationDownMirrored:
        fixUpDown = 1;
        break;
    }
  }

  CFRelease(myImageSource);
  // Make sure the image exists before continuing
  if (!myImage) {
    fprintf(stderr, "Image not created from image source.");
    return false;
  }

  size_t w = CGImageGetWidth(myImage);
  size_t h = CGImageGetHeight(myImage);
  result.xsize = w;
  result.ysize = h;
  result.setFormat(GEM_RGBA);
  result.upsidedown = !fixUpDown;
  result.reallocate();
  result.setBlack();
  CGRect rect = {{0,0},{w,h}};

  CGColorSpaceRef colorSpace;
  CGContextRef context;
  void*data;
  colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);
  if(!colorSpace) {
    goto done;
  }
  context = CGBitmapContextCreate(result.data,
                                  result.xsize, result.ysize, 8, result.xsize * result.csize,
                                  colorSpace, kCGImageAlphaPremultipliedFirst);
  if(!context) {
    goto done;
  }

  CGContextDrawImage(context, rect, myImage);
  if(CGBitmapContextGetData (context) == result.data) {
    success=true;
  }

done:
  if(context) {
    CGContextRelease(context);
  }
  if(colorSpace) {
    CGColorSpaceRelease(colorSpace);
  }
  CFRelease(myImage);
  if (success) {
    result.fixUpDown();
  }
  return success;
}
bool imageIO::save(const imageStruct&img,
                   const std::string&filename, const std::string&mimetype,
                   const gem::Properties&props)
{
  NSString *path = [NSString stringWithUTF8String:filename.c_str()];
  NSURL *url = [NSURL fileURLWithPath:path];
  bool success = false;

  float compression = 1.0; // Lossless compression if available.
  int orientation = kCGImagePropertyOrientationDownMirrored; // Origin is at bottom, left.

  CFStringRef uti = mime2uti(mimetype);

  CFStringRef myKeys[3];
  CFTypeRef   myValues[3];
  CFDictionaryRef myOptions = NULL;
  myKeys[0] = kCGImagePropertyOrientation;
  myValues[0] = CFNumberCreate(NULL, kCFNumberIntType, &orientation);
  myKeys[1] = kCGImagePropertyHasAlpha;
  myValues[1] = kCFBooleanTrue;
  myKeys[2] = kCGImageDestinationLossyCompressionQuality;
  myValues[2] = CFNumberCreate(NULL, kCFNumberFloatType, &compression);
  myOptions = CFDictionaryCreate(NULL, (const void **)myKeys, (const void **)myValues,
                                 sizeof(myKeys)/sizeof(*myKeys),
                                 &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

  CGImageRef myImage = NULL;
  CGImageDestinationRef myImageDest = NULL;
  CGDataProviderRef data = CGDataProviderCreateWithData(NULL,
                           img.data,
                           img.xsize * img.ysize * img.csize,
                           NULL);
  if(!data) {
    goto done;
  }
  myImage = CGImageCreate(img.xsize, img.ysize,
                          8, 8 * img.csize,
                          img.xsize * img.csize,
                          CGColorSpaceCreateDeviceRGB(), CGBitmapInfo(kCGBitmapByteOrderDefault | kCGImageAlphaFirst),
                          data,
                          NULL, false, kCGRenderingIntentDefault);
  if(!myImage) {
    fprintf(stderr, "unable to create CGImage from data\n");
    goto done;
  }

  myImageDest = CGImageDestinationCreateWithURL((CFURLRef)url, uti, 1, nil);
  CGImageDestinationAddImage(myImageDest, myImage, myOptions);
  if(!myImageDest) {
    fprintf(stderr, "unable to create destination image for %s\n", filename.c_str());
    goto done;
  }

  CGImageDestinationFinalize(myImageDest);
  success=true;

done:
  if(myImageDest) {
    CFRelease(myImageDest);
  }
  if(myImage) {
    CFRelease(myImage);
  }
  if(data) {
    CFRelease(data);
  }
  if(myOptions) {
    CFRelease(myOptions);
  }
  return success;
}


float imageIO::estimateSave(const imageStruct&img,
                            const std::string&filename, const std::string&mimetype,
                            const gem::Properties&props)
{
  float result=0;
  CFStringRef uti = mime2uti(mimetype);

  if(uti) {
    result += 100;
  }

#if 0
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
#endif
  return result;
}


void imageIO::getWriteCapabilities(std::vector<std::string>&mimetypes,
                                   gem::Properties&props)
{
  mimetypes.clear();
  props.clear();

  mimetypes.push_back("image/jpeg");
  mimetypes.push_back("image/jp2");
  mimetypes.push_back("image/jpx");
  mimetypes.push_back("image/jpm");
  mimetypes.push_back("image/tiff");
  mimetypes.push_back("image/x-tiff");
  mimetypes.push_back("image/pict");
  mimetypes.push_back("image/x-pict");
  mimetypes.push_back("image/gif");
  mimetypes.push_back("image/png");
  mimetypes.push_back("image/bmp");
  mimetypes.push_back("image/x-windows-bmp");
  mimetypes.push_back("image/x-quicktime");
  mimetypes.push_back("image/x-icon");
  mimetypes.push_back("image/icns");

#if 0
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
#endif
}
