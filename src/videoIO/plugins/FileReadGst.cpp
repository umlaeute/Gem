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
    have_pipeline_(false), playing_(false), new_video_(false)
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

  /// TODO diesen URI Handler noch dazutun, dass man auch streams
  ///      und dvds usw öffnen kann

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

  // NOTE colorspace_ and sink_ are linked in the callback

  gst_element_add_pad (video_bin_, gst_ghost_pad_new ("sink", video_pad));
  gst_object_unref(video_pad);
  gst_bin_add (GST_BIN (file_decode_), video_bin_);

  have_pipeline_=true;

  // set paused state
  gst_element_set_state (file_decode_, GST_STATE_PAUSED);
  playing_=false;
  new_video_=true;

  return true;
}

void FileReadGst::closeFile()
{
  if(!have_pipeline_) return;

  playing_=false;
  gst_element_set_state (file_decode_, GST_STATE_NULL);
  gst_object_unref (GST_OBJECT (file_decode_));
}

void FileReadGst::startVideo()
{
  if(!have_pipeline_) return;

  gst_element_set_state (file_decode_, GST_STATE_PLAYING);
  playing_=true;
}

void FileReadGst::stopVideo()
{
  if(!have_pipeline_) return;

  gst_element_set_state (file_decode_, GST_STATE_PAUSED);
  playing_=false;
}

bool FileReadGst::setPosition(int frame, int track)
{
  if(!have_pipeline_) return false;

  /// TODO schauen warum das nicht funktioniert ???

  /// TODO dann ein schneller/langsamer Spielen auch implementiern
  /// theoretisch muss man dann hier nur den 2. Parameter von 1.0
  /// auf z.B. 2.0 setzen, dann is doppelt so schnell
  /// aber dann auch eine eigene Methode für diese Geschw machen,
  /// die man dann von PD aus auch schön ansprechen kann
  /// z.B. mit einer message [speed 2.0(

  return gst_element_seek(file_decode_, 1.0, GST_FORMAT_DEFAULT,
                 GST_SEEK_FLAG_NONE, GST_SEEK_TYPE_SET, frame,
                 GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
}

unsigned char *FileReadGst::getFrameData()
{
  if(!have_pipeline_) return 0;

  // ATTENTION: calling gst_app_sink_pull_buffer will result in
  // a deadlock if the state is not playing, therefore we
  // have to use this playing_ variable as a hack
  /// TODO Author von appsink nochmal fragen, der hat gesagt
  ///      er macht support für GST_STATE_PAUSED auch dazu !
  if( !playing_ )
    return 0;

  if(!gst_app_sink_end_of_stream (GST_APP_SINK (sink_)))
  {
    // ATTENTION: this will result in a deadlock of the gstreamer
    //            state is not playing !
    GstBuffer *buf = gst_app_sink_pull_buffer(GST_APP_SINK (sink_));
    guint8 *data = GST_BUFFER_DATA( buf );
    guint size = GST_BUFFER_SIZE( buf );

    if( new_video_ )
    {
      GstCaps *caps = gst_buffer_get_caps (buf);
      GstStructure *str = gst_caps_get_structure (caps, 0);

      post("FileReadGst loaded video: %s",
            gst_caps_to_string (caps) );

      // getting fomrat options
      int x_size, y_size, bpp, depth;
      int fps_numerator, fps_denominator;
      g_assert( gst_structure_get_int(str, "width", &x_size) );
      g_assert( gst_structure_get_int(str, "height", &y_size) );
      g_assert( gst_structure_get_fraction (str, "framerate",
              &fps_numerator, &fps_denominator) );

      int format=-1;
      gst_structure_get_int(str, "bpp", &bpp);
      gst_structure_get_int(str, "depth", &depth);
      format = YUV422; // for YUV there's no bpp and depth
      if( bpp==24 && depth==24 ) format=RGB;
      if( bpp==8 && depth==8 ) format=GRAY;
      if( bpp==32 ) format=RGBA;

      // allocate frame
      frame_.allocate(x_size, y_size, format);

      // set framerate
      framerate_ = fps_numerator / fps_denominator;

      /// TODO Frameanzahl bekommt man so nicht -> das könnte man
      /// bei filesrc irgendwie abfragen: bei gst-inspect filesrc
      /// gibts ein "num-buffers" property -> is das nr of frames ?

      new_video_=false;
      gst_caps_unref(caps);
    }
    
   unsigned char *frame = frame_.getFrameData();

   int xs = frame_.getXSize();
   int ys = frame_.getYSize();
   int cs = frame_.getColorSize();

   /// TODO maybe these conversions could be done more efficient !?
   switch( frame_.getColorspace() )
   {
     case YUV422:
     case RGB:
     case GRAY:
       for(int x=0; x<xs; ++x) {
       for(int y=0; y<ys; ++y) {
       for(int c=0; c<cs; ++c) {
         // swap y axis
         frame[y*xs*cs + x*cs + c] =
         data[(ys-y-1)*xs*cs + x*cs + c];
       } } }
       break;

     case RGBA:
       for(int x=0; x<xs; ++x) {
       for(int y=0; y<ys; ++y) {
       for(int c=0; c<cs; ++c) {
         // swap y axis, exchange red and blue
         int c_rgba = (c==0) ? 2 :( (c==2) ? 0 : c );
         frame[y*xs*cs + x*cs + c] =
         data[(ys-y-1)*xs*cs + x*cs + c_rgba];
       } } }
       break;
   }

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

//   post("Callback Caps: %s", gst_caps_to_string (caps) );

  // force a colorspace conversion if requested
  switch(tmp->cspace_)
  {
    case RGBA:
      gst_element_link_filtered(tmp->colorspace_, tmp->sink_,
           gst_caps_new_simple ("video/x-raw-rgb", 
                                "bpp", G_TYPE_INT, 32,
                                "depth", G_TYPE_INT, 32,
                                NULL) );
      break;

    case RGB:
      gst_element_link_filtered(tmp->colorspace_, tmp->sink_,
           gst_caps_new_simple ("video/x-raw-rgb", 
                                "bpp", G_TYPE_INT, 24,
                                "depth", G_TYPE_INT, 24,
                                NULL) );
      break;

    case YUV422:
      gst_element_link_filtered(tmp->colorspace_, tmp->sink_,
           gst_caps_new_simple ("video/x-raw-yuv", 
                                "format", GST_TYPE_FOURCC,
                                GST_MAKE_FOURCC('U', 'Y', 'V', 'Y'),
                                NULL) );
      break;

    case GRAY:
      gst_element_link_filtered(tmp->colorspace_, tmp->sink_,
           gst_caps_new_simple ("video/x-raw-gray", NULL) );
      break;

    default:
      // see if we have YUV, then we have to convert to
      // GEMs YUV format
      GstStructure *str = gst_caps_get_structure (caps, 0);

      // if we not have a "bpp" property we should have YUV
      /// TODO maybe find a better way to see if its YUV
      int bpp;
      if( true)
      if( !gst_structure_get_int(str, "bpp", &bpp) )
      {
        gst_element_link_filtered(tmp->colorspace_, tmp->sink_,
           gst_caps_new_simple ("video/x-raw-yuv", 
                                "format", GST_TYPE_FOURCC,
                                GST_MAKE_FOURCC('U', 'Y', 'V', 'Y'),
                                NULL) );
      }
      else gst_element_link(tmp->colorspace_, tmp->sink_);
  }

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
