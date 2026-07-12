/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2014, Dan Wilcox and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Adapted from the openFrameworks ofAVFoundationVideoPlayer class:
 * Lukasz Karluk, Sam Kronick, James George, & Elie Zananiri 2014.
 *
 * ------------------------------------------------------------------
 */

#import <AVFoundation/AVFoundation.h>
#import <Accelerate/Accelerate.h>
#import <CoreMedia/CoreMedia.h>

// AVFoundation asset reader wrapper
@interface AVFMoviePlayer : NSObject
{

  AVAsset *_asset;
  AVAssetReader *_assetReader;
  AVAssetReaderTrackOutput *_videoTrackOutput;

  BOOL _isLoaded;
  BOOL _isFrameNew;
  int _numFrames;

  unsigned long _desiredPixelFormat;
  CMSampleBufferRef videoSampleBuffer;
}

// has a file been loaded?
@property (nonatomic, readonly) BOOL isLoaded;

// is the current video frame new?
@property (nonatomic, readonly) BOOL isFrameNew;

// desired asset reader pixel format,
// either kCVPixelFormatType_32ARGB or kCVPixelFormatType_422YpCbCr8 (default)
@property (nonatomic) unsigned long desiredPixelFormat;

// loaded file properties
@property (nonatomic, readonly) int numFrames;
@property (nonatomic, readonly) int numTracks;
@property (nonatomic, readonly) int width;
@property (nonatomic, readonly) int height;
@property (nonatomic, readonly) float frameRate;
@property (nonatomic, readonly) float duration;

// open a file at a given path, returns YES on success
// set async = YES to open asynchronously
- (BOOL)openFile:(NSString *)path async:(BOOL)async;

// close currently open file
- (void)close;

// set the current frame and track
// set track = -1 keep current
- (void)setFrame:(int)frame andTrack:(int)track;

// get the current frame data, updates isFrameNew
- (CVImageBufferRef)getFrame;

@end
