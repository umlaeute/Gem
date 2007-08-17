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
     * @param device_number the number of the device to open
     * @return true if successfully opened
     */
    virtual bool openDevice(int device_number) = 0;
    
    /*!
     * opens the device
     * @param device_name the name of the device to open
     * @return true if successfully opened
     */
    virtual bool openDevice(string device_name) = 0;
    
    /// closes the device
    /// @return true if successfully closed
    virtual bool closeDevice() = 0;
    
    virtual void startDevice() {};
    
    virtual void stopDevice() {};
    
    virtual void seekDevice() {};
    
    /// @return the data of the current VIOFrame
    virtual unsigned char *getFrameData() 
      {return frame_.getFrameData();}
    
    /*!
     * sets the channel of the device 
     * (e.g switch between tuner, COMPOSITE)
     * @param channel the desired channel
     * @return true if succsessful
     */
    virtual bool setChannel(int channel);
    
    /*!
     * sets the frequency of the device (e.g for a TV device)
     * @param channel the desired channel
     * @return true if succsessful
     */
    virtual bool setFrequency(float frequency);
    
    /*!
     * sets the TV norm (PAL, NTSC or SECAM)
     * @param norm the TV norm
     * @return true if sccsessful in setting
     */
    virtual bool setTVNorm (char *norm);
    
    /*!
     * sets the output to the desired color model
     * @param colorspace the desired color model
     * @return true if succsessful
     */
    virtual bool forceColorspace (int colorspace) = 0;
    
    
    /// @return the frames per second
    virtual double getFPS()
    {return framerate_;}
    
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
    
    double framerate_;
    
    /// stores the current frame
    VIOFrame frame_ ;
  };
}

#endif
