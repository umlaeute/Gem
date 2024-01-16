/*  -*- mode: ObjC; -*- */
/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

grab live video using AVFoundation

Copyright (c) 2018 IOhannes m zmölnig

Adapted from the openFrameworks ofAVFoundationGrabber class:
Jordan C. Parsons, Paul Gafton, Ryuichi Yamamoto, Seb Lee-Delisle,
Arturo Castro & Theodore Watson 2016-2018

For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
-----------------------------------------------------------------*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "AVFVideoGrabber.h"
#import <Accelerate/Accelerate.h>

#include "Gem/RTE.h"
#include "Gem/PixConvert.h"

#include <iostream>


void printSampleBuffer(CMSampleBufferRef sampleBuffer)
{
  fprintf(stderr, "sampleBuffer: %p\tvalid:%d\tready:%d\n"
          , sampleBuffer
          , CMSampleBufferIsValid(sampleBuffer)
          , CMSampleBufferDataIsReady(sampleBuffer)
         );
  fprintf(stderr, "\tdecode@: %lld\t%lld\n"
          , CMSampleBufferGetDecodeTimeStamp(sampleBuffer).value
          , CMSampleBufferGetOutputDecodeTimeStamp(sampleBuffer).value
         );
  fprintf(stderr, "\tpresen@: %lld\t%lld\n"
          , CMSampleBufferGetPresentationTimeStamp(sampleBuffer).value
          , CMSampleBufferGetOutputPresentationTimeStamp(sampleBuffer).value
         );
};

@interface AVFVideoGrabber ()
@property (nonatomic,retain) AVCaptureSession *captureSession;
@end

@implementation AVFVideoGrabber
@synthesize captureSession;

#pragma mark -
#pragma mark Initialization
- (id)init
{
  self = [super init];
  if (self) {
    captureInput = nil;
    captureOutput = nil;
    device = nil;

    bInitCalled = NO;
    deviceID = 0;
    width = 0;
    height = 0;
  }
  return self;
}

- (BOOL)initCapture:(int)framerate
  capWidth:(int)w capHeight:(int)h
  capFormat:(int)fmt
{
  NSArray * devices = [AVCaptureDevice devicesWithMediaType:
                                       AVMediaTypeVideo];
  switch(fmt) {
/* for whatever reasons, kCVPixelFormatType_32(ABGR,RGBA) give us no images */
/* kCVPixelFormatType_32ARGB gives us a black image (premultiplied alpha?) */
  case GEM_RAW_RGBA:
  case GEM_RAW_BGRA:
    glformat=fmt;
    capformat=kCVPixelFormatType_32BGRA;
    break;
  default:
    glformat=fmt;
    capformat=kCVPixelFormatType_422YpCbCr8;
    break;
  }
  if([devices count] > 0) {
    if(deviceID>[devices count]-1) {
      deviceID = [devices count]-1;
    }
    if(deviceID < 0) {
      deviceID = 0;
    }


    // We set the device
    device = [devices objectAtIndex:deviceID];

    NSError *error = nil;
    [device lockForConfiguration:&error];

    if(!error) {

      float smallestDist = 1e20;
      int bestW, bestH = 0;

      // Set width and height to be passed in dimensions
      // We will then check to see if the dimensions are supported and if not find the closest matching size.
      width = w;
      height = h;

      AVCaptureDeviceFormat * bestFormat  = nullptr;
      for ( AVCaptureDeviceFormat * format in [device formats] ) {
        CMFormatDescriptionRef desc = format.formatDescription;
        CMVideoDimensions dimensions = CMVideoFormatDescriptionGetDimensions(desc);

        int tw = dimensions.width;
        int th = dimensions.height;

        if( tw == width && th == height ) {
          bestW = tw;
          bestH = th;
          bestFormat = format;
          break;
        }

        float dw = tw - width;
        float dh = th = height;

        float dist = (dw*dw) + (dh*dh);
        if( dist < smallestDist ) {
          smallestDist = dist;
          bestW = tw;
          bestH = th;
          bestFormat = format;
        }


        verbose(1, "[GEM:videoAVF] supported dimensions are: %dx%d",
                dimensions.width, dimensions.height);
      }

      // Set the new dimensions and format
      if( bestFormat != nullptr && bestW != 0 && bestH != 0 ) {
        if( bestW != width || bestH != height ) {
          verbose(1,
                  "[GEM:videoAVF] dimension %dx%d not supported. using %dx%d instead", width,
                  height, bestW, bestH);
        }

        [device setActiveFormat:bestFormat];
        width = bestW;
        height = bestH;
      }

      //only set the framerate if it has been set by the user
      if( framerate > 0 ) {

        AVFrameRateRange * desiredRange = [AVFrameRateRange alloc];
        NSArray * supportedFrameRates =
          device.activeFormat.videoSupportedFrameRateRanges;

        int numMatch = 0;
        for(AVFrameRateRange * range in supportedFrameRates) {

          if( (floor(range.minFrameRate) <= framerate
               && ceil(range.maxFrameRate) >= framerate) ) {
            verbose(1,
                    "[GEM:videoAVF] found good framerate range %f .. %f for request %d",
                    range.minFrameRate, range.maxFrameRate, framerate);
            desiredRange = range;
            numMatch++;
          }
        }

        if( numMatch > 0 ) {
          //TODO: this crashes on some devices ( Orbecc Astra Pro )
          device.activeVideoMinFrameDuration = desiredRange.minFrameDuration;
          device.activeVideoMaxFrameDuration = desiredRange.maxFrameDuration;
        } else {
          verbose(1, "[GEM:videoAVF] could not set framerate to %d. Device supports",
                  framerate);
          for(AVFrameRateRange * range in supportedFrameRates) {
            verbose(1, "\t%f .. %f", range.minFrameRate, range.maxFrameRate);
          }
        }
      }

      [device unlockForConfiguration];
    } else {
      NSLog(@"AVFVideoGrabber Init Error: %@", error);
    }

    // We setup the input
    captureInput = [AVCaptureDeviceInput deviceInputWithDevice:device
                                         error:nil];

    // We setup the output
    captureOutput = [[AVCaptureVideoDataOutput alloc] init];
    // While a frame is processed in -captureOutput:didOutputSampleBuffer:fromConnection: delegate methods,
    //   no other frames are added to the queue.
    // If you don't want this behaviour set the property to NO
    captureOutput.alwaysDiscardsLateVideoFrames = YES;

    // We create a serial queue to handle the processing of our frames
    dispatch_queue_t queue;
    queue = dispatch_queue_create("cameraQueue", NULL);
    [captureOutput setSampleBufferDelegate:self queue:queue];
    //dispatch_release(queue);

    NSDictionary* videoSettings =[NSDictionary dictionaryWithObjectsAndKeys:
                                               [NSNumber numberWithDouble:width], (id)kCVPixelBufferWidthKey,
                                               [NSNumber numberWithDouble:height], (id)kCVPixelBufferHeightKey,
                                               [NSNumber numberWithUnsignedInt:capformat],
                                               (id)kCVPixelBufferPixelFormatTypeKey,
                                               nil];
    [captureOutput setVideoSettings:videoSettings];

    // And we create a capture session
    if(self.captureSession) {
      self.captureSession = nil;
    }
    //self.captureSession = [[[AVCaptureSession alloc] init] autorelease];
    self.captureSession = [[AVCaptureSession alloc] init];

    [self.captureSession beginConfiguration];

    // We add input and output
    [self.captureSession addInput:captureInput];
    [self.captureSession addOutput:captureOutput];

    // We specify a minimum duration for each frame (play with this settings to avoid having too many frames waiting
    // in the queue because it can cause memory issues). It is similar to the inverse of the maximum framerate.
    // In this example we set a min frame duration of 1/10 seconds so a maximum framerate of 10fps. We say that
    // we are not able to process more than 10 frames per second.
    // Called after added to captureSession

    AVCaptureConnection *conn = [captureOutput connectionWithMediaType:
                                               AVMediaTypeVideo];
    if ([conn isVideoMinFrameDurationSupported] == YES &&
        [conn isVideoMaxFrameDurationSupported] == YES) {
      [conn setVideoMinFrameDuration:CMTimeMake(1, framerate)];
      [conn setVideoMaxFrameDuration:CMTimeMake(1, framerate)];
    }

    // We start the capture Session
    [self.captureSession commitConfiguration];
    [self.captureSession startRunning];

    bInitCalled = YES;
    return YES;
  }
  return NO;
}

#define SETSUPPORTED(device, prop, value) \
  if( [device is ## prop ## Supported:value] ) \
              [device set ## prop:value ]

-(void) startCapture
{
  [self.captureSession startRunning];

  [captureInput.device lockForConfiguration:nil];

  // SETSUPPORTED(captureInput.device, ExposureMode, AVCaptureExposureModeAutoExpose);
  SETSUPPORTED(captureInput.device, FocusMode, AVCaptureFocusModeAutoFocus);
}

-(void) lockExposureAndFocus
{

  [captureInput.device lockForConfiguration:nil];

  // SETSUPPORTED(captureInput.device, ExposureMode, AVCaptureExposureModeLocked);
  SETSUPPORTED(captureInput.device, FocusMode, AVCaptureFocusModeLocked);
}

-(void)stopCapture
{
  if(self.captureSession) {
    if(captureOutput) {
      if(captureOutput.sampleBufferDelegate != nil) {
        [captureOutput setSampleBufferDelegate:nil queue:NULL];
      }
    }

    // remove the input and outputs from session
    for(AVCaptureInput *input1 in self.captureSession.inputs) {
      [self.captureSession removeInput:input1];
    }
    for(AVCaptureOutput *output1 in self.captureSession.outputs) {
      [self.captureSession removeOutput:output1];
    }

    [self.captureSession stopRunning];
  }
}

-(pixBlock&)getCurrentFrame
{
  return pixes;
}

-(std::vector <std::string>)listDevices
{
  std::vector <std::string> deviceNames;
  NSArray * devices = [AVCaptureDevice devicesWithMediaType:
                                       AVMediaTypeVideo];
  int i=0;
  for (AVCaptureDevice * captureDevice in devices) {
    deviceNames.push_back([captureDevice.localizedName UTF8String]);
    verbose(0, "[GEM:videoAVF] device #%d: %s", i, deviceNames.back().c_str());
    i++;
  }
  return deviceNames;
}

-(void)setDevice:(int)_device
{
  deviceID = _device;
}

#pragma mark -
#pragma mark AVCaptureSession delegate
- (void)captureOutput:(AVCaptureOutput *)captureOutput
  didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
  fromConnection:(AVCaptureConnection *)connection
{
  @autoreleasepool {
    CMTime ts = CMSampleBufferGetPresentationTimeStamp(sampleBuffer);
    if(CMTIME_IS_INVALID(lastSeen) || CMTIME_COMPARE_INLINE(ts, >, lastSeen))
    {
      lastSeen=ts;
      //fprintf(stderr, "new frame @ %lld %d\n", lastSeen.value, CMTIME_IS_VALID(lastSeen));
    } else
    {
      printSampleBuffer(sampleBuffer);
      return;
    }

    CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    // Lock the image buffer
    CVPixelBufferLockBaseAddress(imageBuffer,0);

    unsigned char *isrc4 = (unsigned char *)CVPixelBufferGetBaseAddress(imageBuffer);
    size_t widthIn  = CVPixelBufferGetWidth(imageBuffer);
    size_t heightIn	= CVPixelBufferGetHeight(imageBuffer);

    lock.lock();
    pixes.newimage = true;
    pixes.image.xsize = widthIn;
    pixes.image.ysize = heightIn;

    pixes.image.setFormat(glformat);
    pixes.image.reallocate();
    switch(capformat)
    {
    case kCVPixelFormatType_32BGRA:
      switch(pixes.image.format) {
      case GEM_RAW_BGRA:
         BGRAtoARGB(isrc4, pixes.image.data, width, height); // GEM_RAW_BGRA
         break;
      case GEM_RAW_RGBA:
         BGRAtoABGR(isrc4, pixes.image.data, width, height); // GEM_RAW_RGBA
         break;
      default:
         pixes.image.setBlack();
         break;
      }
      break;
    case kCVPixelFormatType_422YpCbCr8:
      pixes.image.fromYUV422(isrc4);
      break;
    default:
      pixes.image.setBlack();
      break;
    }
    lock.unlock();

    // Unlock the image buffer
    CVPixelBufferUnlockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);
  }
}

#pragma mark -
#pragma mark Memory management

- (void)dealloc
{
  // Stop the CaptureSession
  if(self.captureSession) {
    [self stopCapture];
    self.captureSession = nil;
  }
  if(captureOutput) {
    if(captureOutput.sampleBufferDelegate != nil) {
      [captureOutput setSampleBufferDelegate:nil queue:NULL];
    }
    //[captureOutput release];
    captureOutput = nil;
  }

  captureInput = nil;
  device = nil;

  //[super dealloc];
}

@end
