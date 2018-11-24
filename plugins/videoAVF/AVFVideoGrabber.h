/*
 *  AVFVideoGrabber.h
 */

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

  CGImageRef currentFrame;

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
