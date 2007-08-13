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

namespace VideoIO_
{
  class StreamRead{
  public:
    
    /// constructor
    StreamRead();
    
    /// destructor
    virtual ~StreamRead();
  
    /*!
     * opens the V4L device
     * @param device_number the number of the device
     * @param format the requested format
     * @return true if the device was successfully opened
     */
    virtual bool  openDevice(int device_number, int format=0){return false;};
    

    /// closes the device
    virtual void  closeDevice(void){};
    
    /*!
     * resets the device
     * @return true if the device was successfully reset
     */
    virtual bool  resetDevice(void){return false;};
  
    /*!
     * Start up the video device (called on startRendering)
     * @param format the requested format
     * @return false if transfer doesn't work
     */
    virtual bool  startTransfer(int format = 0){return false;};
    
    /*!
     * Stops the transfer
     * @return true if successful
     */
    virtual bool  stopTransfer(){return false;};
  
    /// @return a VIOFrame of the video stream
    virtual VIOFrame *getFrame(){};
  
    /// sets the video dimensions
    virtual bool setDimen(int x_size, int y_size, int left_margin=0, int right_margin=0,
                                  int top_margin=0, int bottom_margin=0){return false;};
    /// TODO wird das benötigt ??
    /// virtual int setOffset(int x, int y);
    virtual bool setSwap(int state){return false;}; /// Was ist das?
    virtual bool setChannel(int channel, float f=0){return false;};
    virtual bool setTVNorm(string tv_norm){return false;};
    virtual bool setDevice(int device_number){return false;};
    virtual bool setDevice(string device_name){return false;};
    virtual bool setColorspace(int){return false;};
    virtual bool setQuality(int){return false;};
  
  protected:
    bool capturing_;
    bool has_video_;
    VIOFrame frame_;
    
    int width_;
    int height_;
    int channel_;
    int tv_norm_;
    int req_format_;
    int quality_;
    
    /* specify either devicename XOR devicenum */  
    string device_name_;
    int device_number_;
  
  
    
  };
}
#endif
