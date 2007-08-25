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
    source_(NULL), videorate_(NULL), colorspace_(NULL), vsink_(NULL),
    file_decode_(NULL), video_bin_(NULL), bus_(NULL), adapter_(NULL),
    audio_bin_(NULL), 
    have_pipeline_(false), new_video_(false)
{
}

FileReadGst::~FileReadGst()
{
  if(!have_pipeline_) return;

  // Gstreamer clean up
  gst_element_set_state (file_decode_, GST_STATE_NULL);
  gst_object_unref (GST_OBJECT (file_decode_));
  g_object_unref( adapter_ );
}

bool FileReadGst::openFile(string filename)
{
  initGstreamer();
  closeFile();

  // make new adapter
  if( adapter_ ) g_object_unref( adapter_ );
  adapter_ = gst_adapter_new();

  string uri = getURIFromFilename(filename);

  // pipeline
  file_decode_ = gst_pipeline_new( "file_decode_");

  // source+decode
  source_ = gst_element_factory_make("uridecodebin", "source_");
  g_assert(source_);

  g_object_set (G_OBJECT(source_), "uri", uri.c_str(), NULL);
  gst_bin_add (GST_BIN (file_decode_), source_);
      
  g_signal_connect (source_, "pad-added", G_CALLBACK (cbNewpad), (gpointer)this);
  
  have_pipeline_=true;
  
  // creating video output bin
  video_bin_ = gst_bin_new ("video_bin_");
  g_assert(video_bin_);
  videorate_ = gst_element_factory_make("videorate", "videorate_");
  g_assert(videorate_);
  colorspace_ = gst_element_factory_make ("ffmpegcolorspace", "colorspace_");
  g_assert(colorspace_);
  vqueue_ = gst_element_factory_make ("queue", "vqueue_");
  g_assert(vqueue_);
  vsink_ = gst_element_factory_make ("appsink", "vsink_");
  g_assert(vsink_);
  
  gst_bin_add_many (GST_BIN (video_bin_), videorate_, colorspace_, vqueue_, vsink_, NULL);
  // NOTE colorspace_ and vqueue_ are linked in the callback
  gst_element_link(videorate_, colorspace_);
  gst_element_link(vqueue_, vsink_);

  GstPad *video_pad = gst_element_get_pad (videorate_, "sink");
  gst_element_add_pad (video_bin_, gst_ghost_pad_new ("sink", video_pad));
  gst_object_unref(video_pad);
  gst_bin_add (GST_BIN (file_decode_), video_bin_);
  
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
  if(!have_pipeline_) 
  {
    post("no working pipeline.");
    return;
  }

  if(!gst_element_set_state (file_decode_, GST_STATE_PLAYING))
    post("The state could not be set to playing.");
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
  
  int seek_flags =  GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT;
  gint64 seek_pos = (gint64) (sec * GST_MSECOND);

  if( !gst_element_seek_simple( file_decode_, GST_FORMAT_TIME,
      (GstSeekFlags) seek_flags, seek_pos ) )
  {
    post("videoIO: seek not possible");
    return false;
  }

  return true;
}

void FileReadGst::setSpeed(float speed)
{
  if (speed == 0)
  {
    post("videoIO: 0 is an invalid speed.");
    return;
  }
  
  int seek_flags =  GST_SEEK_FLAG_FLUSH | GST_SEEK_FLAG_KEY_UNIT;
  
  if (!gst_element_seek(GST_ELEMENT(file_decode_), speed,
                       GST_FORMAT_UNDEFINED, (GstSeekFlags)seek_flags,
                       GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE,
                       GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE))
    post("videoIO: speed could not be set.");
}

unsigned char *FileReadGst::getFrameData()
{
  if(!have_pipeline_) return 0;

  if( gst_app_sink_end_of_stream( GST_APP_SINK (vsink_) ) ) return 0;

  GstBuffer *buf = 0;
  
//   post("GST_STATE: %d, GST_STATE_PENDING: %d",
//         GST_STATE(file_decode_), GST_STATE_PENDING(file_decode_));

  // get the buffer in PLAYING state
  if( GST_STATE(file_decode_)==GST_STATE_PLAYING &&
      GST_STATE_PENDING(file_decode_)==GST_STATE_VOID_PENDING )
  {
    buf = gst_app_sink_pull_buffer(GST_APP_SINK (vsink_));
  }

  // in PAUSED state get the preroll buffer
  if( GST_STATE(file_decode_)==GST_STATE_PAUSED ||
      (GST_STATE(file_decode_)==GST_STATE_PLAYING &&
       GST_STATE_PENDING(file_decode_)==GST_STATE_PAUSED) )
  {
    buf = gst_app_sink_pull_preroll(GST_APP_SINK (vsink_));
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

void FileReadGst::getAudioBlock(t_float *left, t_float *right, int n)
{
  // if there is no audio bin or the pipeline isn't in playing state
  if(!audio_bin_ || !(GST_STATE(file_decode_)==GST_STATE_PLAYING &&
                      GST_STATE_PENDING(file_decode_)==GST_STATE_VOID_PENDING))
  {
    // write zero samples in audio block
    while(n--)
    {
      *left++ = 0;
      *right++ = 0;
    }
    return;
  }

  // if we have end of stream send zero samples
  if(gst_app_sink_end_of_stream( GST_APP_SINK (asink_)))  ///TODO scheint nicht richtig zu funktionieren
  {
    while(n--)
    {
      *left++ = 0;
      *right++ = 0;
    }
    return;
  }

  GstBuffer *buf = 0;

  //samples*channels*bytesperdata
  // (we have converted to 32 bit data = 4 bytes)
  int buffersize = n * 2 * 4;

  // check if we need to get a new buffer
  if( gst_adapter_available(adapter_) < buffersize )
  {
    buf = gst_app_sink_pull_buffer(GST_APP_SINK (asink_));
    if( !buf ) post("------------------");

//     GstCaps *caps = gst_buffer_get_caps (buf);
//     post("FileReadGst loaded audioframe: %s",
//           gst_caps_to_string (caps) );

    gst_adapter_push (adapter_, buf);
  }

  if( gst_adapter_available(adapter_) >= buffersize )
  {
    t_float *data = (t_float*) gst_adapter_peek (adapter_, buffersize);

//     post("data123: %f %f %f", data[0], data[2], data[4]);

    for(int i=0; i<n; ++i)
    {
      left[i] = data[i*2];
      right[i] = data[i*2+1];
    }

    gst_adapter_flush (adapter_, buffersize);
  }
  else
  {
    post("FileReadGst audio dropout");

    // write zero samples in audio block
    while(n--)
    {
      *left++ = 0;
      *right++ = 0;
    }
  }
}

bool FileReadGst::createAudioBin()
{
  // creating audio output bin
  audio_bin_ = gst_bin_new ("audio_bin_");
  g_assert(audio_bin_);
  aconvert_ = gst_element_factory_make("audioconvert", "aconvert_");
  g_assert(aconvert_);
  aresample_ = gst_element_factory_make ("audioresample", "aresample_");
  g_assert(aresample_);
  aqueue_ = gst_element_factory_make ("queue", "aqueue_");
  g_assert(aqueue_);
  asink_ = gst_element_factory_make ("appsink", "asink_");
  g_assert(asink_);
  
  gst_bin_add_many (GST_BIN (audio_bin_), aconvert_, aresample_, aqueue_, asink_, NULL);
  gst_element_link_many(aconvert_, aresample_, aqueue_, NULL);
  /// TODO get framerate of pd
  gst_element_link_filtered(aqueue_, asink_,
    gst_caps_new_simple ("audio/x-raw-float",
                         "rate", G_TYPE_INT, 44100,
                         "channels", G_TYPE_INT, 2,
                         "width", G_TYPE_INT, 32,
//                          "endianness", G_TYPE_INT, G_BIG_ENDIAN,
                         NULL) );

  GstPad *audio_pad = gst_element_get_pad (aconvert_, "sink");
  gst_element_add_pad (audio_bin_, gst_ghost_pad_new ("sink", audio_pad));
  gst_object_unref(audio_pad);
  gst_bin_add (GST_BIN (file_decode_), audio_bin_);
  
  gst_element_set_state (audio_bin_, GST_STATE_PAUSED);
  
  return true;
}

bool FileReadGst::createVideoBin()  ///TODO funktioniert im Moment nicht
{
//   // creating video output bin
//   video_bin_ = gst_bin_new ("video_bin_");
//   g_assert(video_bin_);
//   videorate_ = gst_element_factory_make("videorate", "videorate_");
//   g_assert(videorate_);
//   colorspace_ = gst_element_factory_make ("ffmpegcolorspace", "colorspace_");
//   g_assert(colorspace_);
//   vqueue_ = gst_element_factory_make ("queue", "vqueue_");
//   g_assert(vqueue_);
//   vsink_ = gst_element_factory_make ("appsink", "vsink_");
//   g_assert(vsink_);
//   
//   gst_bin_add_many (GST_BIN (video_bin_), videorate_, colorspace_, vqueue_, vsink_, NULL);
//   // NOTE colorspace_ and vqueue_ are linked in the callback
//   gst_element_link(videorate_, colorspace_);
//   gst_element_link(vqueue_, vsink_);
// 
//   GstPad *video_pad = gst_element_get_pad (videorate_, "sink");
//   gst_element_add_pad (video_bin_, gst_ghost_pad_new ("sink", video_pad));
//   gst_object_unref(video_pad);
//   gst_bin_add (GST_BIN (file_decode_), video_bin_);
//   
//   gst_element_set_state (audio_bin_, GST_STATE_PAUSED);
//   
  return true;
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
  GstStructure *vstr;
  GstPad *videopad = NULL, *audiopad = NULL;
  
  bool link_audio = false, link_video = false;
  
  FileReadGst *tmp = (FileReadGst *)data;
    
  // check if we have a video and/or audio
  caps = gst_pad_get_caps (pad);
  vstr = gst_caps_get_structure (caps, 0);
  if (g_strrstr (gst_structure_get_name (vstr), "video"))
    link_video = true;
  if (g_strrstr (gst_structure_get_name (vstr), "audio")) 
    link_audio = true;
  
  if(link_video)
  {
//     if(!tmp->createVideoBin())
//       post("FileReadGst: The video bin could not be created");
    
    videopad = gst_element_get_pad (tmp->video_bin_, "sink");
    g_assert(videopad);
    
     // check if the pads are already linked
    if( GST_PAD_IS_LINKED (videopad) )
    {
      g_object_unref (videopad);
      link_video = false;            ///TODO warum auf false setzen??
    }
  }
  
  if(link_audio)
  {
    // the audio bin will only be created if needed
    if(!tmp->createAudioBin())
      post("FileReadGst: The audio bin could not be created");
    
    audiopad = gst_element_get_pad (tmp->audio_bin_, "sink");
    g_assert(audiopad);
    
    if ( GST_PAD_IS_LINKED (audiopad) )
    {
      g_object_unref (audiopad);
      link_audio = false;
    }
  }
  

  post("Callback Caps: %s", gst_caps_to_string (caps) );

  if( link_video )
  {
	// force a colorspace conversion if requested
	int fr1 = (int) tmp->fr_host_ * 10000;
	int fr2 = 10000;
	switch(tmp->cspace_)
	{
	case RGBA:
	gst_element_link_filtered(tmp->colorspace_, tmp->vqueue_,
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
	gst_element_link_filtered(tmp->colorspace_, tmp->vqueue_,
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
	gst_element_link_filtered(tmp->colorspace_, tmp->vqueue_,
		gst_caps_new_simple ("video/x-raw-yuv", 
					"format", GST_TYPE_FOURCC,
					GST_MAKE_FOURCC('U', 'Y', 'V', 'Y'),
					"framerate", GST_TYPE_FRACTION, fr1, fr2,
					NULL) );
	break;
	
	case GRAY:
	gst_element_link_filtered(tmp->colorspace_, tmp->vqueue_,
		gst_caps_new_simple ("video/x-raw-gray",
					"framerate", GST_TYPE_FRACTION, fr1, fr2,
					NULL) );
	break;
	
	default:
	// see if we have YUV, then we have to convert to
	// GEMs YUV format
	GstStructure *vstr = gst_caps_get_structure (caps, 0);
	
	// if we not have a "bpp" property we should have YUV
	/// TODO maybe find a better way to see if its YUV
	int bpp;
	if( !gst_structure_get_int(vstr, "bpp", &bpp) )
	{
		gst_element_link_filtered(tmp->colorspace_, tmp->vqueue_,
		gst_caps_new_simple ("video/x-raw-yuv", 
					"format", GST_TYPE_FOURCC,
					GST_MAKE_FOURCC('U', 'Y', 'V', 'Y'),
					"framerate", GST_TYPE_FRACTION, fr1, fr2,
					NULL) );
	}
	else // make framerate conversion
	{
		gst_element_link_filtered(tmp->colorspace_, tmp->vqueue_,
		gst_caps_new_simple ("video/x-raw-rgb",
					"framerate", GST_TYPE_FRACTION, fr1, fr2,
					NULL) );
	}
	}
	
	// link the pads
	gst_pad_link (pad, videopad);
  }

  if( link_audio )
  {
    gst_pad_link (pad, audiopad);
  }


  // CLEANUP
  gst_caps_unref (caps);
  gst_object_unref (videopad);
  gst_object_unref (audiopad);
  return;
}

/// Tells us to register our functionality to an engine kernel
void registerPlugin(VIOKernel &K)
{
  K.getFileReadServer().addPlugin(
    auto_ptr<FileRead>(new FileReadGst()));
  
  post("VideoIO: registerd FileReadGst Plugin");
}
