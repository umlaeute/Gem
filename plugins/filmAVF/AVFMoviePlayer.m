/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Implementation file

Copyright (c) 2016-2018 Dan Wilcox.

Adapted from the openFrameworks ofAVFoundationVideoPlayer class:
Lukasz Karluk, Sam Kronick, James George, & Elie Zananiri 2014.

For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/
#import "AVFMoviePlayer.h"

// private stuff
@interface AVFMoviePlayer()
@property (nonatomic, strong) AVAsset *asset;
@property (nonatomic, strong) AVAssetReader *assetReader;
@property (nonatomic, strong) AVAssetReaderTrackOutput *videoTrackOutput;
@property (nonatomic, readwrite) BOOL isLoaded;
@property (nonatomic, readwrite) BOOL isFrameNew;
@property (nonatomic, readwrite) int numFrames;
@end

/////////////////////////////////////////////////////////
//
// AVFMoviePlayer
//
/////////////////////////////////////////////////////////
@implementation AVFMoviePlayer

@synthesize asset = _asset;
@synthesize assetReader = _assetReader;
@synthesize videoTrackOutput = _videoTrackOutput;
@synthesize isLoaded = _isLoaded;
@synthesize isFrameNew = _isFrameNew;
@synthesize numFrames = _numFrames;
@synthesize desiredPixelFormat = _desiredPixelFormat;

/////////////////////////////////////////////////////////
// init
//
/////////////////////////////////////////////////////////
- (id)init
{
  self = [super init];
  if(self) {
    self.desiredPixelFormat = kCVPixelFormatType_422YpCbCr8;
    videoSampleBuffer = nil;
  }
  return self;
}

/////////////////////////////////////////////////////////
// dealloc
//
/////////////////////////////////////////////////////////
- (void)dealloc
{
  [self close];
}

/////////////////////////////////////////////////////////
// openFile:async:
//
/////////////////////////////////////////////////////////
- (BOOL)openFile:(NSString *)path async:(BOOL)async
{
  [self close];

  NSURL *url = [NSURL fileURLWithPath:path];
  NSDictionary *options = @ {(id)AVURLAssetPreferPreciseDurationAndTimingKey : @YES};
  AVURLAsset *asset = [AVURLAsset URLAssetWithURL:url options:options];
  if(asset == nil) {
    return NO;
  }

  // queue for aysnc load or block until done
  dispatch_semaphore_t done = dispatch_semaphore_create(0);
  dispatch_queue_t queue;
  if(async) {
    queue = dispatch_get_main_queue();
  } else {
    queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
  }

  dispatch_async(queue, ^ {
    [asset loadValuesAsynchronouslyForKeys:@[@"tracks"] completionHandler:^{
            NSError *error = nil;

            // check tracks
      AVKeyValueStatus status = [asset statusOfValueForKey:@"tracks" error:&error];
      if(status != AVKeyValueStatusLoaded)
      {
        NSLog(@"AVFMoviePlayer: error loading asset tracks: %@",
              error.localizedDescription);
        if(!async) {
          dispatch_semaphore_signal(done);
        }
        return;
      }

      // check duration
      if(CMTimeCompare(asset.duration, kCMTimeZero) == 0)
      {
        NSLog(@"AVFMoviePlayer: track loaded with zero duration");
        if(!async) {
          dispatch_semaphore_signal(done);
        }
        return;
      }

      // check video tracks
      NSArray *videoTracks = [asset tracksWithMediaType:AVMediaTypeVideo];
      if(videoTracks.count == 0)
      {
        NSLog(@"AVFMoviePlayer: no video tracks found");
        if(!async) {
          dispatch_semaphore_signal(done);
        }
        return;
      }

      // asset is good
      self.asset = asset;

      // create asset reader
      if(![self createAssetReaderWithTimeRange:CMTimeRangeMake(kCMTimeZero, self.asset.duration)])
      {
        if(!async) {
          dispatch_semaphore_signal(done);
        }
        return;
      }
      self.numFrames = floor(self.duration * self.frameRate);

      // done
      self.isLoaded = YES;
      if(!async)
      {
        dispatch_semaphore_signal(done);
      }
    }];
  });

  // wait for the done semaphore signal
  if(async) {
    return YES;
  } else { // done
    dispatch_semaphore_wait(done, DISPATCH_TIME_FOREVER);
    return self.isLoaded;
  }
}

/////////////////////////////////////////////////////////
// close
//
/////////////////////////////////////////////////////////
- (void)close
{

  // free
  [self.assetReader cancelReading];
  self.assetReader = nil;
  self.videoTrackOutput = nil;
  self.asset = nil;
  if(videoSampleBuffer) {
    CFRelease(videoSampleBuffer);
    videoSampleBuffer = nil;
  }

  // defaults
  self.isLoaded = NO;
  self.isFrameNew = NO;
  self.numFrames = 0;
}

/////////////////////////////////////////////////////////
// setFrame:andTrack:
//
/////////////////////////////////////////////////////////
- (void)setFrame:(int)frame andTrack:(int)track
{
  if(!self.isLoaded) {
    return;
  }

  // set frame
  float position = frame / (float)self.numFrames;
  double t = self.duration * position;
  CMTime time = CMTimeMakeWithSeconds(t, NSEC_PER_SEC);

  // create asset reader at specific time, restrict time within 0-duration
  time = CMTimeMaximum(time, kCMTimeZero);
  time = CMTimeMinimum(time, self.asset.duration);
  [self createAssetReaderWithTimeRange:CMTimeRangeMake(time,
           self.asset.duration)];
}

/////////////////////////////////////////////////////////
// getFrame
//
/////////////////////////////////////////////////////////
- (CVImageBufferRef)getFrame
{
  if(self.videoTrackOutput != nil
      && self.assetReader.status == AVAssetReaderStatusReading) {
    CMSampleBufferRef newVideoSampleBuffer = nil;
    @try {
      newVideoSampleBuffer = [self.videoTrackOutput copyNextSampleBuffer];
    } @catch(NSException *e) {
      NSLog(@"AVFMoviePlayer: error copying video sample buffer: %@", e);
    }
    if(newVideoSampleBuffer) {
      if(videoSampleBuffer) { // release old buffer
        CFRelease(videoSampleBuffer);
        videoSampleBuffer = nil;
      }
      videoSampleBuffer = newVideoSampleBuffer; // save reference to new buffer
      newVideoSampleBuffer = nil;
      self.isFrameNew = YES;
    } else {
      self.isFrameNew = NO;
    }
  }
  return CMSampleBufferGetImageBuffer(videoSampleBuffer);
}

#pragma mark Private

/////////////////////////////////////////////////////////
// createAssetReaderWithTimeRange:
// create video output at specific time
/////////////////////////////////////////////////////////
- (BOOL)createAssetReaderWithTimeRange:(CMTimeRange)timeRange
{

  // clear
  [self.assetReader cancelReading];
  self.assetReader = nil;
  self.videoTrackOutput = nil;

  // create new asset reader
  NSError *error = nil;
  self.assetReader = [AVAssetReader assetReaderWithAsset:self.asset error:
                                    &error];
  if(error) {
    NSLog(@"assetReader: error during initialization: %@",
          error.localizedDescription);
    return NO;
  }
  self.assetReader.timeRange = timeRange;

  // https://developer.apple.com/reference/avfoundation/avassetreadertrackoutput?language=objc
  // kCVPixelFormatType_422YpCbCr8 (fastest) or kCVPixelFormatType_32BGRA
  NSDictionary *settings = @ {(NSString *)kCVPixelBufferPixelFormatTypeKey : [NSNumber numberWithInt:self.desiredPixelFormat]};
  AVAssetTrack *videoTrack = [[self.asset tracksWithMediaType:
                               AVMediaTypeVideo] firstObject];
  self.videoTrackOutput = [AVAssetReaderTrackOutput
                           assetReaderTrackOutputWithTrack:videoTrack outputSettings:settings];
  if(self.videoTrackOutput != nil) {
    self.videoTrackOutput.alwaysCopiesSampleData = NO;
    if([self.assetReader canAddOutput:self.videoTrackOutput]) {
      [self.assetReader addOutput:self.videoTrackOutput];
    } else {
      NSLog(@"AVFMoviePlayer: could not add video track output to asset reader");
      return NO;
    }
  } else {
    NSLog(@"AVFMoviePlayer: could not create video track output");
    return NO;
  }

  // start reading
  if(![self.assetReader startReading]) {
    NSLog(@"AVFMoviePlayer: asset reader could not start reading: %@",
          self.assetReader.error.localizedDescription);
    return NO;
  }

  return YES;
}

#pragma mark Overridden Getters/Setters

- (void)setDesiredPixelFormat:(unsigned long) format
{
  if(_desiredPixelFormat == format) {
    return;
  }
  _desiredPixelFormat = format;
  if(self.assetReader) {
    // recreate asset reader with new format
    [self createAssetReaderWithTimeRange:self.assetReader.timeRange];
  }
}

- (int)numTracks
{
  return self.asset == nil ? 0 : [[self.asset tracksWithMediaType:
                                   AVMediaTypeVideo] count];
}

- (int)width
{
  AVAssetTrack *videoTrack = [[self.asset tracksWithMediaType:
                               AVMediaTypeVideo] objectAtIndex:0];
  return videoTrack == nil ? 0 : videoTrack.naturalSize.width;
}

- (int)height
{
  AVAssetTrack *videoTrack = [[self.asset tracksWithMediaType:
                               AVMediaTypeVideo] objectAtIndex:0];
  return videoTrack == nil ? 0: videoTrack.naturalSize.height;
}

- (float)frameRate
{
  AVAssetTrack *videoTrack = [[self.asset tracksWithMediaType:
                               AVMediaTypeVideo] objectAtIndex:0];
  return videoTrack == nil ? 0 : videoTrack.nominalFrameRate;
}

- (float)duration
{
  return self.asset == nil ? 0 : CMTimeGetSeconds(self.asset.duration);
}

@end
