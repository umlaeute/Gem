//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   Loads a video stream (like AVI, Mpeg, Quicktime) into a VIOFrame.
//
//   StreamRead
//   header file
//
//   copyright            : (C) 2007 by Thomas Holzmann
//   email                : holzi1@gmx.at
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
///////////////////////////////////////////////////////////////////////////

#ifndef STREAM_READ_H_
#define STREAM_READ_H_

using namespace std;

#include "VIOUtils.h"
#include "VIOFrame.h"
#include "VIOException.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
	video
    
	a OS-indendent parent-class for retrieving video-frames
	
KEYWORDS
	pix
    
DESCRIPTION

    "dimen" (int, int) - set the x,y dimensions
    "zoom" (int, int) - the zoom factor (1.0 is nominal) (num / denom)
    "bright" (int) - the brightnes
    "contrast" (int) - the contrast
    "hue" (int) - the hue
    "sat" (int) - the saturation
    
-----------------------------------------------------------------*/
namespace VideoIO_
{
  class StreamRead{
  public:
    
    //////////
    // Constructor
    StreamRead(){};
    
    //////////
    // Destructor
    virtual ~StreamRead(){};
  
    ////////
    // open the video-device
    virtual void  openDevice(int devnum, int format=0){};
    virtual void  closeDevice(void){};
    virtual void  resetDevice(void){};
  
    //////////
    // Start up the video device (called on startRendering)
    // [out] int - returns 0 if bad
    virtual void  startTransfer(int format = 0){};
    //////////
    // Stop the video device (called on stopRendering)
    // [out] int - returns 0 if bad
    virtual int  stopTransfer(){};
  
    //////////
    // get the next frame (called when rendering)
    virtual VIOFrame *getFrame(){};
  
    //////////
    // Set the video dimensions
    virtual int setDimen(int x, int y, int left_margin=0, int right_margin=0,
                                  int top_margin=0, int bottom_margin=0){};
    /// TODO wird das benötigt ??
    /// virtual int setOffset(int x, int y);
    virtual int setSwap(int state){};
    virtual int setChannel(int channel, float f=0){};
    virtual int setTVNorm(string tv_norm){};
    virtual int setDevice(int device_number){};
    virtual int setDevice(string device_name){};
    virtual int setColor(int){};
    virtual int setQuality(int){};
  
  protected:
    bool capturing_;
    bool has_video_;
    VIOFrame frame_;
    
    int width_;
    int height_;
    int channel_;
    int tv_norm_;
    int req_format_;
  
    /* specify either devicename XOR devicenum */  
    string device_name_;
    int device_number_;
  
  
    int quality_;
  };
}
#endif	// for header file
