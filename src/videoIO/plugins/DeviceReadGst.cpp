//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The gstreamer device read plugin.
//
//   DeviceReadGst
//   implementation file
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

#include "DeviceReadGst.h"

bool DeviceReadGst::is_initialized_ = false;

bool DeviceReadGst::openDevice(string device_name)
{
  /// TODO: in den konstruktor bzw statische methode geben !!!
  if(!is_initialized_)
  {
    if(gst_init_check (NULL, NULL, NULL))  
    {
      is_initialized_ = true;
    }
    else
    {
      post("Gstreamer could not be initialised. is_initialised: %d", is_initialized_);
      return false;
    }
  }
  
  post ("This program is linked against %s", gst_version_string());
  
  // setup pipeline
  device_decode_ = gst_pipeline_new( "device_decode_");
  source_ = gst_element_factory_make ("v4lsrc", "source_");
  g_assert(source_);
  colorspace_ = gst_element_factory_make ("ffmpegcolorspace", "colorspace_");
  g_assert(colorspace_);
  sink_ = gst_element_factory_make ("appsink", "sink_");
  g_assert(sink_);
  
  gst_bin_add_many (GST_BIN(device_decode_), source_, colorspace_, sink_);
  
  gst_element_link (source_, colorspace_);
  gst_element_link_filtered(colorspace_, sink_, gst_caps_new_simple ("video/x-raw-rgb", NULL));
  
  
  // set playing state
  if(!gst_element_set_state (device_decode_, GST_STATE_PLAYING))
  {
    post("The state could not be set to ready.");
    return false;
  }

  return true;
  
}

/// Tells us to register our functionality to an engine kernel
void registerPlugin(VIOKernel &K)
{
  K.getDeviceReadServer().addPlugin(
    auto_ptr<DeviceRead>(new DeviceReadGst()) );
  
  post("VideoIO: registerd DeviceReadGst Plugin");
}
