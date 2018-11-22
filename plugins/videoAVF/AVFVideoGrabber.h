/*
 *  AVFVideoGrabber.h
 */

#pragma once

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <Accelerate/Accelerate.h>
#import <CoreMedia/CoreMedia.h>
#import <CoreVideo/CoreVideo.h>

class videoAVF;

@interface AVFVideoGrabber : NSObject
  <AVCaptureVideoDataOutputSampleBufferDelegate>
{

@public
  CGImageRef currentFrame;

  int width;
  int height;

  BOOL bInitCalled;
  int deviceID;

  AVCaptureDeviceInput *captureInput;
  AVCaptureVideoDataOutput *captureOutput;
  AVCaptureDevice *device;
  AVCaptureSession *captureSession;

  videoAVF *grabberPtr;
}

-(BOOL)initCapture:(int)framerate capWidth:(int)w capHeight:(int)h;
-(void)startCapture;
-(void)stopCapture;
-(void)lockExposureAndFocus;
-(std::vector <std::string>)listDevices;
-(void)setDevice:(int)_device;
-(void)eraseGrabberPtr;

-(CGImageRef)getCurrentFrame;

@end
