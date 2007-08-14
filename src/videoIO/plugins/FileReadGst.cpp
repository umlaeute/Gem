//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The plugin for GStreamer.
//
//   FileReadGst
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

#include "FileReadGst.h"


bool FileReadGst::is_initialized_ = false;

FileReadGst::FileReadGst() :
    source_(NULL), decode_(NULL), colorspace_(NULL), sink_(NULL),
    file_decode_(NULL), video_bin_(NULL), bus_(NULL),
    have_pipeline_(false)
{

}

FileReadGst::~FileReadGst()
{
  if(!have_pipeline_) return;

  // Gstreamer clean up
  gst_element_set_state (file_decode_, GST_STATE_NULL);
  gst_object_unref (GST_OBJECT (file_decode_));
}

bool FileReadGst::openFile(string filename)
{
  initGstreamer();

  // pipeline
  file_decode_ = gst_pipeline_new( "file_decode_");

  // source+decode
  source_ = gst_element_factory_make("filesrc", "source_");
  g_assert(source_);
  decode_ = gst_element_factory_make ("decodebin", "decode_");
  g_assert(decode_);
  
  g_object_set (G_OBJECT(source_), "location", filename.c_str(), NULL);
  gst_bin_add_many (GST_BIN (file_decode_), source_, decode_, NULL);
  gst_element_link (source_, decode_);
      
  g_signal_connect (decode_, "new-decoded-pad", G_CALLBACK (cbNewpad), (gpointer)this);
  
  // creating video output
  video_bin_ = gst_bin_new ("video_bin_");
  g_assert(video_bin_);
  colorspace_ = gst_element_factory_make ("ffmpegcolorspace", "colorspace_");
  g_assert(colorspace_);
  sink_ = gst_element_factory_make ("appsink", "sink_");
  g_assert(sink_);
  
  GstPad *video_pad = gst_element_get_pad (colorspace_, "sink");
  
  gst_bin_add_many (GST_BIN (video_bin_), colorspace_, sink_, NULL);
  /// TODO forceColorspace hier einbauen !?
  gst_element_link_filtered(colorspace_, sink_, gst_caps_new_simple ("video/x-raw-rgb", NULL));
  gst_element_add_pad (video_bin_, gst_ghost_pad_new ("sink", video_pad));
  gst_object_unref(video_pad);
  gst_bin_add (GST_BIN (file_decode_), video_bin_);

  have_pipeline_=true;
  

  // set paused state
  gst_element_set_state (file_decode_, GST_STATE_PLAYING);

  return true;
}

void FileReadGst::closeFile()
{
  if(!have_pipeline_) return;

  gst_element_set_state (file_decode_, GST_STATE_NULL);
  gst_object_unref (GST_OBJECT (file_decode_));
}

void FileReadGst::startVideo()
{
  if(have_pipeline_)
    gst_element_set_state (file_decode_, GST_STATE_PLAYING);
}

void FileReadGst::stopVideo()
{
  if(!have_pipeline_) return;

  gst_element_set_state (file_decode_, GST_STATE_PAUSED);
}

bool FileReadGst::setPosition(int frame, int track)
{
  //if(have_pipeline_)
  /// TODO seeken implementieren
}

unsigned char *FileReadGst::getFrameData()
{
  if(!have_pipeline_) return 0;

  if( GST_STATE (file_decode_) != GST_STATE_PLAYING )
    return 0;

  if(!gst_app_sink_end_of_stream (GST_APP_SINK (sink_)))
  {
    // ATTENTION: this will result in a deadlock of the gstreamer
    //            state is not playing !
    GstBuffer *buf = gst_app_sink_pull_buffer(GST_APP_SINK (sink_));
    guint8 *data = GST_BUFFER_DATA( buf );
    guint size = GST_BUFFER_SIZE( buf );
    
    post("Caps: %s", gst_caps_to_string (gst_buffer_get_caps (buf)));
        
    unsigned char *frame = frame_.getFrameData();

    // copy and swap data for gstreamer
    while(size--)
      *frame++ = *data++;

    gst_buffer_unref (buf);
  }
  
  return frame_.getFrameData();
}

void FileReadGst::initGstreamer()
{
  if(is_initialized_) return;

  gst_init(NULL,NULL);
  is_initialized_=true;
}

void FileReadGst::cbNewpad(GstElement *decodebin, GstPad *pad, 
                           gboolean last, gpointer data)
{
  GstCaps *caps;
  GstStructure *str;
  GstPad *videopad;
  
  FileReadGst *tmp = (FileReadGst *)data;

  // only link once
  videopad = gst_element_get_pad (tmp->video_bin_, "sink");
  g_assert(videopad);
  
  if (GST_PAD_IS_LINKED (videopad)) {
    g_object_unref (videopad);
    return;
  }

  // check if we have a video
  caps = gst_pad_get_caps (pad);
  str = gst_caps_get_structure (caps, 0);
  if (!g_strrstr (gst_structure_get_name (str), "video")) 
  {
    gst_caps_unref (caps);
    gst_object_unref (videopad);
    return;
  }
  
  // getting fomrat options
  int x_size, y_size, bpp, depth;
  double fps;
  //g_assert( gst_structure_get_int(str, "width", &x_size) );
//   g_assert( gst_structure_get_int(str, "height", &y_size) );
//   g_assert( gst_structure_get_int(str, "bpp", &bpp) );
//   g_assert( gst_structure_get_int(str, "depth", &depth) );
//   g_assert( gst_structure_get_double (str, "framerate", &fps) );

  /// TODO mimetype noch kriegen und vom frame dann die
  ///      properties setzen
  tmp->frame_.allocate(320, 240, RGB);

  gst_caps_unref (caps);

  // link the pads
  gst_pad_link (pad, videopad);
}

/// Tells us to register our functionality to an engine kernel
void registerPlugin(VIOKernel &K)
{
  K.getFileReadServer().addPlugin(
    auto_ptr<FileRead>(new FileReadGst()));
  
  post("VideoIO: registerd FileReadGst Plugin");
}
