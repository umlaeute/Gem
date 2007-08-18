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
  ~DeviceReadGst();

  /*!
   * opens the device
   * @param name can be "video" (capture card, webcam, etc.)
   *             or "dv" (dv input)
   * @param device the device (e.g. /dev/video0), optional
   * @return true if successfully opened
   */
  virtual bool openDevice(const string &name, const string &device="");
  
  /// closes the device
  /// @return true if successfully closed
  virtual bool closeDevice();
  
  /// starts grabbing data from the device
  virtual void startDevice();
  
  /// stops grabbing data from the device
  virtual void stopDevice();

  /// @return the data of the current VIOFrame
  virtual unsigned char *getFrameData();

 protected:
  GstElement *source_;
  GstElement *demux_;
  GstElement *decode_;
  GstElement *videorate_;
  GstElement *colorspace_;
  GstElement *sink_;
  GstElement *device_decode_;
  
  bool have_pipeline_;
  bool new_device_;

  void setupDVPipeline();
  void setupV4LPipeline(const string &device);
  void freePipeline();
  
  static void initGstreamer();
  static bool is_initialized_;
  
  static void cbNewpad(GstElement *element, GstPad *pad, gpointer data);
  static void cbFrameDropped(GstElement *element, gpointer data);
};

/// Tells us to register our functionality to an engine kernel
extern "C" void registerPlugin(VIOKernel &K);

#endif
