//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The plugin for GStreamer.
//
//   FileWriteGst
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

#include "FileWriteGst.h"


bool FileWriteGst::is_initialized_ = false;

FileWriteGst::FileWriteGst()
{

}

FileWriteGst::~FileWriteGst()
{

}

bool FileWriteGst::initRecording()
{
  /// TODO Auflösung, CS, ... automatisch erkennen (vom pixImage aus pix_file_write::render)
  gst_app_src_set_caps ( GST_APP_SRC(source_),
                       gst_caps_new_simple ("video/x-raw-rgb",
				     "width", G_TYPE_INT, x_size_,
				     "height", G_TYPE_INT, y_size_,
				     "framerate", GST_TYPE_FRACTION, 20, 1,
				     "bpp", G_TYPE_INT, 24,
				     "endianness", G_TYPE_INT, G_BYTE_ORDER,
				     NULL)
                       );
  
  return true;
}

bool FileWriteGst::stopRecording()
{
  gst_app_src_end_of_stream ( GST_APP_SRC(source_) );
  
  return true;
}

void FileWriteGst::pushFrame(VIOFrame &frame)
{
//   GstBuffer *buf;
//   buf = gst_app_buffer_new (data, 100, dont_eat_my_chicken_wings, data);
//   gst_app_src_push_buffer (GST_APP_SRC (source_), buf);
}

bool FileWriteGst::openFile(string filename)
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
  file_encode_ = gst_pipeline_new( "file_encode_");
  
  // creating video input
  video_bin_ = gst_bin_new ("video_bin_");
  g_assert(video_bin_);
  
  source_ = gst_element_factory_make ("appsrc", "source_");
  g_assert(source_);
  colorspace_ = gst_element_factory_make ("ffmpegcolorspace", "colorspace_");
  g_assert(colorspace_);
  
  GstPad *video_pad = gst_element_get_pad (colorspace_, "source");
  
  gst_bin_add_many (GST_BIN (video_bin_), source_, colorspace_, NULL);
  gst_element_link(source_, colorspace_);
  gst_element_add_pad (video_bin_, gst_ghost_pad_new ("source", video_pad));
  gst_object_unref(video_pad);
  
  encode_ = gst_element_factory_make ("theoraenc", "encode_");
  g_assert(encode_);
  sink_ = gst_element_factory_make ("filesink", "sink_");
  g_assert(sink_);
  
  g_object_set (G_OBJECT(sink_), "location", filename.c_str(), NULL);
  
  gst_bin_add_many (GST_BIN (file_encode_), video_bin_, encode_, sink_, NULL);
  gst_element_link_many (video_bin_, encode_, sink_, NULL);
  
    // set playing state
  if(!gst_element_set_state (file_encode_, GST_STATE_READY))
  {
    post("The state could not be set to ready.");
    return false;
  }

  return true;
}


/// Tells us to register our functionality to an engine kernel
void registerPlugin(VIOKernel &K)
{
  K.getFileWriteServer().addPlugin(
    auto_ptr<FileWrite>(new FileWriteGst()));
  
  post("VideoIO: registerd FileWriteGst Plugin");
}

