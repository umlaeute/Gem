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
    file_decode_(NULL), video_bin_(NULL), bus_(NULL), x_size_(0),
    y_size_(0), fps_(0.)
{

}

FileReadGst::~FileReadGst()
{
  // Gstreamer clean up
  gst_element_set_state (file_decode_, GST_STATE_NULL);
  gst_object_unref (GST_OBJECT (file_decode_));
}

unsigned char *FileReadGst::getFrameData()
{
#ifdef DEBUG
  error("FileReadGst::getFrameData begin");
#endif
  
  if(!gst_app_sink_end_of_stream (GST_APP_SINK (sink_)))
  {
    GstBuffer *buf = gst_app_sink_pull_buffer (GST_APP_SINK (sink_));
    guint8 *data = GST_BUFFER_DATA( buf );
    guint size = GST_BUFFER_SIZE( buf );
    unsigned char *frame = frame_.getFrameData();
    
    while(size--)
      *frame++ = *data++;

    gst_buffer_unref (buf);
  }
  
  return frame_.getFrameData();
}

void FileReadGst::cbNewpad(GstElement *decodebin, GstPad *pad, 
                           gboolean last, gpointer data)
{
  GstCaps *caps;
  GstStructure *str;
  GstPad *videopad;
  
  FileReadGst *tmp = (FileReadGst *)data;

  /* only link once */
  videopad = gst_element_get_pad (tmp->video_bin_, "sink");
  g_assert(videopad);
  
  if (GST_PAD_IS_LINKED (videopad)) {
    g_object_unref (videopad);
    return;
  }
 
  /* check media type */
  caps = gst_pad_get_caps (pad);
  str = gst_caps_get_structure (caps, 0);
  if (!g_strrstr (gst_structure_get_name (str), "video")) 
  {
    gst_caps_unref (caps);
    gst_object_unref (videopad);
    return;
  }
  
  // getting width and height
  int x_size, y_size;
  double fps;
  if (!gst_structure_get_int (str, "width", &x_size) ||
      !gst_structure_get_int (str, "height", &y_size)) 
  {
    y_size =576; ///TODO weil get y_size nicht richtig funktioniert
    //return;
  }
  
//   if(!gst_structure_get_double (str, "framerate", &fps))
//     post ("Could not get framerate.");
  
  fps = 25.;  ///TODO how to get framerate??
  
  tmp->setVideoProperties (x_size, y_size, fps);
  post("FileReadGst: Video with width: %d, height: %d and framerate: %f loaded", x_size, y_size, fps);
  
  gst_caps_unref (caps);

  /* link'n'play */
  gst_pad_link (pad, videopad);
  
  // allocate frame data
  /// TODO check the colorspace and allocate the right size
  tmp->frame_.allocate(x_size, y_size, RGB);
}

bool FileReadGst::openFile(string filename)
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
  file_decode_ = gst_pipeline_new( "file_decode_");
  source_ = gst_element_factory_make ("filesrc", "source_");
  g_assert(source_);
  decode_ = gst_element_factory_make ("decodebin2", "decode_");
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
  gst_element_link(colorspace_, sink_);
  gst_element_add_pad (video_bin_, gst_ghost_pad_new ("sink", video_pad));
  gst_object_unref(video_pad);
  gst_bin_add (GST_BIN (file_decode_), video_bin_);
  
  // set playing state
  if(!gst_element_set_state (file_decode_, GST_STATE_PLAYING))
  {
    post("The state could not be set to ready.");
    return false;
  }

  has_video_file_=true;
  return true;
  
}

void FileReadGst::setVideoProperties(int x_size, int y_size, double fps)
{
  x_size_ = x_size;
  y_size_ = y_size;
  fps_ = fps;
}
  
/// Tells us to register our functionality to an engine kernel
void registerPlugin(VIOKernel &K)
{
  K.getFileReadServer().addFileReadPlugin(
    auto_ptr<FileRead>(new FileReadGst()));
  
  post("VideoIO: registerd FileReadGst Plugin");
}

