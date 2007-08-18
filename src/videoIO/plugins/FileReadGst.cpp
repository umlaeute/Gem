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
    source_(NULL), videorate_(NULL), colorspace_(NULL), sink_(NULL),
    file_decode_(NULL), video_bin_(NULL), bus_(NULL),
    have_pipeline_(false), new_video_(false)
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

  string uri = getURIFromFilename(filename);

  // pipeline
  file_decode_ = gst_pipeline_new( "file_decode_");

  // source+decode
  source_ = gst_element_factory_make("uridecodebin", "source_");
  g_assert(source_);

  g_object_set (G_OBJECT(source_), "uri", uri.c_str(), NULL);
  gst_bin_add (GST_BIN (file_decode_), source_);
      
  g_signal_connect (source_, "pad-added", G_CALLBACK (cbNewpad), (gpointer)this);
  
  // creating video output
  video_bin_ = gst_bin_new ("video_bin_");
  g_assert(video_bin_);
  videorate_ = gst_element_factory_make("videorate", "videorate_");
  g_assert(source_);
  colorspace_ = gst_element_factory_make ("ffmpegcolorspace", "colorspace_");
  g_assert(colorspace_);
  sink_ = gst_element_factory_make ("appsink", "sink_");
  g_assert(sink_);
  
  GstPad *video_pad = gst_element_get_pad (videorate_, "sink");
  
  gst_bin_add_many (GST_BIN (video_bin_), videorate_, colorspace_, sink_, NULL);

  // NOTE colorspace_ and sink_ are linked in the callback
  gst_element_link(videorate_, colorspace_);

  gst_element_add_pad (video_bin_, gst_ghost_pad_new ("sink", video_pad));
  gst_object_unref(video_pad);
  gst_bin_add (GST_BIN (file_decode_), video_bin_);

  have_pipeline_=true;

  // set paused state
  gst_element_set_state (file_decode_, GST_STATE_PAUSED);
  new_video_=true;
  
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
  if(!have_pipeline_) return;

  gst_element_set_state (file_decode_, GST_STATE_PLAYING);
}

void FileReadGst::stopVideo()
{
  if(!have_pipeline_) return;

  gst_element_set_state (file_decode_, GST_STATE_PAUSED);
}

bool FileReadGst::setPosition(float sec)
{
  if(!have_pipeline_) return false;

  if( sec<0 || sec>(duration_-0.1) )
  {
    post("seek position out of range");
    return false;
  }

  int seek_flags = GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_FLUSH;
  gint64 seek_pos = (gint64) (sec * GST_SECOND);

  if( !gst_element_seek_simple( file_decode_, GST_FORMAT_TIME,
      (GstSeekFlags) seek_flags, seek_pos ) )
  {
    post("videoIO: seek not possible");
    return false;
  }

  return true;

  /// TODO dann ein schneller/langsamer Spielen auch implementiern
  /// theoretisch muss man dann hier nur den 2. Parameter von 1.0
  /// auf z.B. 2.0 setzen, dann is doppelt so schnell
  /// aber dann auch eine eigene Methode für diese Geschw machen,
  /// die man dann von PD aus auch schön ansprechen kann
  /// z.B. mit einer message [speed 2.0(
}

unsigned char *FileReadGst::getFrameData()
{
  if(!have_pipeline_) return 0;

  if( gst_app_sink_end_of_stream(GST_APP_SINK (sink_)) ) return 0;

  GstBuffer *buf = 0;
  
//   post("GST_STATE: %d, GST_STATE_PENDING: %d",
//         GST_STATE(file_decode_), GST_STATE_PENDING(file_decode_));

  // get the buffer in PLAYING state
  if( GST_STATE(file_decode_)==GST_STATE_PLAYING &&
      GST_STATE_PENDING(file_decode_)==GST_STATE_VOID_PENDING )
  {
    buf = gst_app_sink_pull_buffer(GST_APP_SINK (sink_));
  }

  // in PAUSED state get the preroll buffer
  if( GST_STATE(file_decode_)==GST_STATE_PAUSED ||
      (GST_STATE(file_decode_)==GST_STATE_PLAYING &&
       GST_STATE_PENDING(file_decode_)==GST_STATE_PAUSED) )
  {
    buf = gst_app_sink_pull_preroll(GST_APP_SINK (sink_));
  }

  if( !buf ) return 0;

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
    g_assert( gst_structure_get_int(str, "width", &x_size) );
    g_assert( gst_structure_get_int(str, "height", &y_size) );

    int format=-1;
    gst_structure_get_int(str, "bpp", &bpp);
    gst_structure_get_int(str, "depth", &depth);
    format = YUV422; // for YUV there's no bpp and depth
    if( bpp==24 && depth==24 ) format=RGB;
    if( bpp==8 && depth==8 ) format=GRAY;
    if( bpp==32 ) format=RGBA;
    gst_caps_unref(caps);

    // allocate frame
    frame_.allocate(x_size, y_size, format);

    // set framerate
    /// TODO try to find a way in gstreamer how to query
    ///      the original framerate of the movie here
    framerate_ = fr_host_;

    // get duration of the video
    GstQuery *query = gst_query_new_duration (GST_FORMAT_TIME);
    bool res = gst_element_query (file_decode_, query);
    if (res)
    {
      gint64 duration;
      gst_query_parse_duration (query, NULL, &duration);
      duration_ = duration / GST_SECOND;
    }
    else post("videoIO: duration query failed");
    gst_query_unref (query);

    new_video_=false;
  }
    
  unsigned char *frame = frame_.getFrameData();

  int xs = frame_.getXSize();
  int ys = frame_.getYSize();
  int cs = frame_.getColorSize();

  for(int x=0; x<xs; ++x)
  for(int y=0; y<ys; ++y)
  for(int c=0; c<cs; ++c) {
    // swap y axis
    frame[y*xs*cs + x*cs + c] =
    data[(ys-y-1)*xs*cs + x*cs + c];
  }

  gst_buffer_unref (buf);
  
  return frame_.getFrameData();
}

string FileReadGst::getURIFromFilename(const string &filename)
{
  string str;

  /// TODO how to handle this correct for gstreamer on windows ?

  // prepend "file://" to a file-system path
  if( filename.compare(0, 1, "/") == 0 )
    str = "file://" + filename;
  else
    str = filename;
  
  return str;
}

void FileReadGst::initGstreamer()
{
  if(is_initialized_) return;

  gst_init(NULL,NULL);
  is_initialized_=true;
}

void FileReadGst::cbNewpad(GstElement *decodebin, GstPad *pad, gpointer data)
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
  int fr1 = (int) tmp->fr_host_ * 10000;
  int fr2 = 10000;
  switch(tmp->cspace_)
  {
    case RGBA:
      gst_element_link_filtered(tmp->colorspace_, tmp->sink_,
           gst_caps_new_simple ("video/x-raw-rgb", 
                                "bpp", G_TYPE_INT, 32,
                                "depth", G_TYPE_INT, 32,
				"red_mask",   G_TYPE_INT, 0xff000000,
				"green_mask", G_TYPE_INT, 0x00ff0000,
				"blue_mask",  G_TYPE_INT, 0x0000ff00,
				"alpha_mask", G_TYPE_INT, 0x000000ff,
                                "framerate", GST_TYPE_FRACTION, fr1, fr2,
                                NULL) );
      break;

    case RGB:
      gst_element_link_filtered(tmp->colorspace_, tmp->sink_,
           gst_caps_new_simple ("video/x-raw-rgb", 
                                "bpp", G_TYPE_INT, 24,
                                "depth", G_TYPE_INT, 24,
				"red_mask",   G_TYPE_INT, 0x00ff0000,
				"green_mask", G_TYPE_INT, 0x0000ff00,
				"blue_mask",  G_TYPE_INT, 0x000000ff,
                                "framerate", GST_TYPE_FRACTION, fr1, fr2,
                                NULL) );
      break;

    case YUV422:
      gst_element_link_filtered(tmp->colorspace_, tmp->sink_,
           gst_caps_new_simple ("video/x-raw-yuv", 
                                "format", GST_TYPE_FOURCC,
                                GST_MAKE_FOURCC('U', 'Y', 'V', 'Y'),
                                "framerate", GST_TYPE_FRACTION, fr1, fr2,
                                NULL) );
      break;

    case GRAY:
      gst_element_link_filtered(tmp->colorspace_, tmp->sink_,
           gst_caps_new_simple ("video/x-raw-gray",
                                "framerate", GST_TYPE_FRACTION, fr1, fr2,
                                NULL) );
      break;

    default:
      // see if we have YUV, then we have to convert to
      // GEMs YUV format
      GstStructure *str = gst_caps_get_structure (caps, 0);

      // if we not have a "bpp" property we should have YUV
      /// TODO maybe find a better way to see if its YUV
      int bpp;
      if( !gst_structure_get_int(str, "bpp", &bpp) )
      {
        gst_element_link_filtered(tmp->colorspace_, tmp->sink_,
           gst_caps_new_simple ("video/x-raw-yuv", 
                                "format", GST_TYPE_FOURCC,
                                GST_MAKE_FOURCC('U', 'Y', 'V', 'Y'),
                                "framerate", GST_TYPE_FRACTION, fr1, fr2,
                                NULL) );
      }
      else // make framerate conversion
      {
        gst_element_link_filtered(tmp->colorspace_, tmp->sink_,
           gst_caps_new_simple ("video/x-raw-rgb",
                                "framerate", GST_TYPE_FRACTION, fr1, fr2,
                                NULL) );
      }
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
