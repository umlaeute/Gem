//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The base class of the DeviceRead plugins.
//
//   DeviceRead
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

#ifndef DEVICE_READ_
#define DEVICE_READ_

using namespace std;

#include <string>

#include "VIOUtils.h"
#include "VIOFrame.h"

namespace VideoIO_
{
  class DeviceRead
  {
    public:
      
    /// constructor
    DeviceRead();
    
    /// destructor
    virtual ~DeviceRead();

    /*!
     * opens the device
     * @param name can be "video" (capture card, webcam, etc.)
     *             or "dv" (dv input)
     * @param device the device (e.g. /dev/video0), optional
     * @return true if successfully opened
     */
    virtual bool openDevice(const string &name, const string &device="") = 0;
    
    /// closes the device
    /// @return true if successfully closed
    virtual bool closeDevice() = 0;
    
    /// starts grabbing data from the device
    virtual void startDevice() = 0;
    
    /// stops grabbing data from the device
    virtual void stopDevice() = 0;
    
    /// seeking: e.g. a DV cam
    virtual void seekDevice(int seek);
    
    /// @return the data of the current VIOFrame
    virtual unsigned char *getFrameData() = 0;
    
    /*!
     * sets the DV decoding quality
     * @param quality between 0 (fastest) and 5 (best)
     */
    virtual void setDVQuality(int quality);

    /*!
     * sets the output to the desired color model
     * @param colorspace the desired color model
     * @return true if succsessful
     */
    virtual bool forceColorspace(int cs)
    { cspace_ = cs; }

    /// set framerate of the host application
    virtual void setFramerate(float fr)
    { framerate_ = fr; }


    /// @return the width of the video
    virtual int getWidth()
    {return frame_.getXSize();}
    
    /// @return the height of the video
    virtual int getHeight()
    {return frame_.getYSize();}
    
    /// @return the colorspace
    virtual int getColorspace()
    {return frame_.getColorspace();}
    
    /// @return the size of a pixel for this colorspace
    virtual int getColorSize()
    {return frame_.getColorSize();}
        
    protected:
    
    ///force a specific colorspace
    int cspace_;
    /// DV quality
    int dv_quality_;
    /// framerate of the host
    float framerate_;

    /// stores the current frame
    VIOFrame frame_ ;
  };
}

#endif
