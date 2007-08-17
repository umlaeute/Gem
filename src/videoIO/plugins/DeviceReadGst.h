//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The gstreamer device read plugin.
//
//   DeviceReadGst
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

#ifndef DEVICE_READ_GST_
#define DEVICE_READ_GST_

#include "../DeviceRead.h"
#include "../VIOKernel.h"
#include "gst/gst.h"
#include "gst/app/gstappsink.h"
#include "gst/app/gstappbuffer.h"

#include <string>

using namespace std;
using namespace VideoIO_;

class DeviceReadGst : public DeviceRead
{
  public:
      
  /// constructor
  DeviceReadGst();
  
  /// destructor
  ~DeviceReadGst(){};
  
  /*!
    * opens the device
    * @param device_number the number of the device to open
    * @return true if successfully opened
    */
  bool openDevice(int device_number){};
  
  /*!
    * opens the device
    * @param device_name the name of the device to open
    * @return true if successfully opened
    */
  bool openDevice(string device_name);
  
  /// closes the device
  /// @return true if successfully closed
  bool closeDevice(){};
  
  void startDevice();
  
  void stopDevice();
  
  /// @return the data of the current VIOFrame
  unsigned char *getFrameData();
  
  /*!
    * sets the channel of the device 
    * (e.g switch between tuner, COMPOSITE)
    * @param channel the desired channel
    * @return true if succsessful
    */
  bool setChannel(int channel){};
  
  /*!
    * sets the frequency of the device (e.g for a TV device)
    * @param channel the desired channel
    * @return true if succsessful
    */
  bool setFrequency(float frequency){};
  
  /*!
    * sets the TV norm (PAL, NTSC or SECAM)
    * @param norm the TV norm
    * @return true if sccsessful in setting
    */
  bool setTVNorm (char *norm){};
  
  /*!
    * sets the output to the desired color model
    * @param colorspace the desired color model
    * @return true if succsessful
    */
  bool forceColorspace (int colorspace){};
  
  
  /// @return the frames per second
  double getFPS(){};
  
  /// @return the width of the video
  int getWidth(){};
  
  /// @return the height of the video
  int getHeight(){};
  
  protected:
  GstElement *source_;
  GstElement *demux_;
  GstElement *decode_;
  GstElement *colorspace_;
  GstElement *sink_;
  GstElement *device_decode_;
  
  bool have_pipeline_;
  bool new_device_;
  
  static bool is_initialized_;
  
  static void cbNewpad(GstElement *element, GstPad *pad, gboolean last, gpointer data);
};

/// Tells us to register our functionality to an engine kernel
extern "C" void registerPlugin(VIOKernel &K);

#endif
