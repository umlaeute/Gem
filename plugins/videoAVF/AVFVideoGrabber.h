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

#pragma once
#include <Gem/Image.h>

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <Accelerate/Accelerate.h>
#import <CoreMedia/CoreMedia.h>
#import <CoreVideo/CoreVideo.h>

#include <mutex>
#include <vector>
#include <string>



typedef void (callback_t)(void *x);

@interface AVFVideoGrabber : NSObject
  <AVCaptureVideoDataOutputSampleBufferDelegate>
{

@public
  pixBlock pixes;
  std::mutex lock;

  int width;
  int height;
  unsigned int capformat;
  int glformat;

  BOOL bInitCalled;
  int deviceID;

  AVCaptureDeviceInput *captureInput;
  AVCaptureVideoDataOutput *captureOutput;
  AVCaptureDevice *device;
  AVCaptureSession *captureSession;

  CMTime lastSeen;
}

-(BOOL)initCapture:(int)framerate capWidth:(int)w capHeight:
  (int)h capFormat:(int)fmt;
-(void)startCapture;
-(void)stopCapture;
-(void)lockExposureAndFocus;
-(std::vector <std::string>)listDevices;
-(void)setDevice:(int)_device;

-(pixBlock&)getCurrentFrame;

@end
