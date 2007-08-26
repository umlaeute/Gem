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
#include <locale.h>

bool FileWriteGst::is_initialized_ = false;

FileWriteGst::FileWriteGst() : new_video_(false), have_pipeline_(false), frame_number_(0)
{
  initGstreamer();
}

FileWriteGst::~FileWriteGst()
{
  if(have_pipeline_)
    freePipeline();
}

void FileWriteGst::pushFrame(VIOFrame &frame)
{
  if(!have_pipeline_) return;

  if(new_video_)
  {
    initRecording( frame.getXSize(), frame.getYSize(), 
                   frame.getColorspace() );
    new_video_=false;
  }

  int cs = frame.getColorSize();
  int xs = frame.getXSize();
  int ys = frame.getYSize();
  int size = ys * GST_ROUND_UP_4( xs * cs ); // for gstreamer buffers

  unsigned char *data = frame.getFrameData();
  unsigned char *rec_data = new unsigned char[size];
  unsigned char *tmp = rec_data;

  // copy data
  for(int x=0; x<xs; ++x)
    for(int y=0; y<ys; ++y)
      for(int c=0; c<cs; ++c) {
        // swap y axis
        tmp[y*xs*cs + x*cs + c] =
        data[(ys-y-1)*xs*cs + x*cs + c];
      }

  GstBuffer *buf;
  buf = gst_app_buffer_new (rec_data, size, freeRecBuffer, (void*)rec_data);

/// TODO müssen wir timestamp und buffer setzen? bei mir hats mit timestamp nicht funktioniert
//   // adding the timestamp to the buffer
//   GstClock *clock = gst_pipeline_get_clock(GST_PIPELINE(file_encode_));
//   GstClockTime time = gst_clock_get_time(clock);
//   buf->timestamp = time;
  
//   // setting the framenumber of this buffer
//   buf->offset = frame_number_;
  
//   // adding caps to the buffer
//   GstCaps *caps = gst_caps_new_simple ("video/x-raw-yuv",
//                                        "format", GST_TYPE_FOURCC, GST_MAKE_FOURCC('U', 'Y', 'V', 'Y'),
//                                        "framerate", GST_TYPE_FRACTION, 20, 1,
//                                        "width", G_TYPE_INT, xs,
//                                        "height", G_TYPE_INT, ys,
//                                        NULL);
//   gst_buffer_set_caps (buf, caps);
  
  gst_app_src_push_buffer (GST_APP_SRC (source_), buf);
  frame_number_++;
}

bool FileWriteGst::openFile(const string &filename)
{


  if(have_pipeline_)
    freePipeline();

  if (codec_ == "ogg" || codec_ == "theora")
    setupOggPipeline(filename);
  else if (codec_ == "mpeg4")   
    setupMpeg4Pipeline(filename);
  else if (codec_ == "udp")
    setupUdpPipeline(filename);
  else
    setupRawPipeline(filename);
  
  ///TODO I only succeeded in playing the mpeg4 and raw files with mplayer, 
  /// vlc doesn't play it. But gstreamer (playbin) can also play it
  
  // set ready state
  if(!gst_element_set_state (file_encode_, GST_STATE_READY))
  {
    post("The state could not be set to READY");
    return false;
  }

  have_pipeline_=true;
  new_video_=true;
  
  setlocale(LC_NUMERIC, "C"); 
  
  return true;
}

bool FileWriteGst::stopRecording()
{
  if(!have_pipeline_) return false;

  gst_app_src_end_of_stream ( GST_APP_SRC(source_) );
  gst_element_set_state (file_encode_, GST_STATE_NULL);

  post("FileWriteGst: stopped recording");
  
  frame_number_ = 0;
  
  return true;
}

void FileWriteGst::initRecording(int xsize, int ysize, int cs)
{
  int fr1 = (int) framerate_ * 10000;
  int fr2 = 10000;

  /// NOTE endianess is set to G_BIG_ENDIAN because this is needed
  /// for some internal reasons (also for Intels !)
  switch(cs)
  {
    case GRAY:
      gst_app_src_set_caps ( GST_APP_SRC(source_),
                       gst_caps_new_simple ("video/x-raw-gray",
				     "width", G_TYPE_INT, xsize,
				     "height", G_TYPE_INT, ysize,
				     "bpp", G_TYPE_INT, 8,
				     "depth", G_TYPE_INT, 8,
			             "framerate", GST_TYPE_FRACTION, fr1, fr2,
				     "endianness", G_TYPE_INT, G_BIG_ENDIAN,
				     NULL)
                       );
      break;

    case YUV422:
      gst_app_src_set_caps ( GST_APP_SRC(source_),
                       gst_caps_new_simple ("video/x-raw-yuv",
			             "framerate", GST_TYPE_FRACTION, fr1, fr2,
				     "width", G_TYPE_INT, xsize,
				     "height", G_TYPE_INT, ysize,
                                     "format", GST_TYPE_FOURCC,
                                     GST_MAKE_FOURCC('U', 'Y', 'V', 'Y'),
				     NULL)
                       );
      break;

    case RGB:
      gst_app_src_set_caps ( GST_APP_SRC(source_),
                       gst_caps_new_simple ("video/x-raw-rgb",
				     "width", G_TYPE_INT, xsize,
				     "height", G_TYPE_INT, ysize,
				     "bpp", G_TYPE_INT, 24,
				     "depth", G_TYPE_INT, 24,
				     "red_mask",   G_TYPE_INT, 0x00ff0000,
				     "green_mask", G_TYPE_INT, 0x0000ff00,
				     "blue_mask",  G_TYPE_INT, 0x000000ff,
			             "framerate", GST_TYPE_FRACTION, fr1, fr2,
				     "endianness", G_TYPE_INT, G_BIG_ENDIAN,
				     NULL)
                       );
      break;

    case RGBA:
    default:
      gst_app_src_set_caps ( GST_APP_SRC(source_),
                       gst_caps_new_simple ("video/x-raw-rgb",
				     "width", G_TYPE_INT, xsize,
				     "height", G_TYPE_INT, ysize,
				     "bpp", G_TYPE_INT, 32,
				     "depth", G_TYPE_INT, 32,
				     "red_mask",   G_TYPE_INT, 0xff000000,
				     "green_mask", G_TYPE_INT, 0x00ff0000,
				     "blue_mask",  G_TYPE_INT, 0x0000ff00,
				     "alpha_mask", G_TYPE_INT, 0x000000ff,
			             "framerate", GST_TYPE_FRACTION, fr1, fr2,
				     "endianness", G_TYPE_INT, G_BIG_ENDIAN,
				     NULL)
                       );
      break;
  }

  // set playing state
  if(!gst_element_set_state (file_encode_, GST_STATE_PLAYING))
    error("FileWriteGst: recording could not be started!");
  else post("FileWriteGst: started recording");
}

void FileWriteGst::getCodec()
{
  post("-----------------------------------------");
  post("FileWriteGst available codecs:");
  post("raw");
  post("theora:");
  post("  quality: 0 - 63, default: 16");
  post("  bitrate: 0 - 2000");
  post("mpeg4: ");
  post("  bitrate: 0 - 4294967295, default: 300000");
  post("-----------------------------------------");
}

bool FileWriteGst::setupRawPipeline(const string &filename)
{
  file_encode_ = gst_pipeline_new( "file_encode_");

  source_ = gst_element_factory_make ("appsrc", "source_");
  g_assert(source_);
//   videorate_ = gst_element_factory_make ("videorate", "videorate_");
//   g_assert(videorate_);
  colorspace_ = gst_element_factory_make ("ffmpegcolorspace", "colorspace_");
  g_assert(colorspace_);
  mux_ = gst_element_factory_make("avimux", "mux_");
  g_assert(mux_);
  queue_ = gst_element_factory_make("queue", "queue_");
  g_assert(queue_);
  sink_ = gst_element_factory_make ("filesink", "sink_");
  g_assert(sink_);

  g_object_set (G_OBJECT(sink_), "location", filename.c_str(), NULL);
  g_object_set (G_OBJECT(sink_), "sync", true, NULL);

  gst_bin_add_many (GST_BIN (file_encode_), source_, colorspace_, mux_, queue_, sink_, NULL);
  gst_element_link_many (source_, colorspace_, mux_, queue_, sink_, NULL);

}

bool FileWriteGst::setupOggPipeline(const string &filename)
{
  /// Test-Pipeline for gst-launch:
  /// gst-launch filesrc location=input.avi ! decodebin !
  /// ffmpegcolorspace ! theoraenc ! oggmux ! filesink location=output.ogg

  file_encode_ = gst_pipeline_new( "file_encode_");

  source_ = gst_element_factory_make ("appsrc", "source_");
  g_assert(source_);
  colorspace_ = gst_element_factory_make ("ffmpegcolorspace", "colorspace_");
  g_assert(colorspace_);
  sink_ = gst_element_factory_make ("filesink", "sink_");
  g_assert(sink_);

  g_object_set (G_OBJECT(sink_), "location", filename.c_str(), NULL);

  encode_ = gst_element_factory_make ("theoraenc", "encode_");
  g_assert(encode_);
  mux_ = gst_element_factory_make("oggmux", "mux_");
  g_assert(mux_);

  // set theora parameter
  if( cparameters_.find("quality") != cparameters_.end() )
    g_object_set (G_OBJECT(encode_), "quality", cparameters_["quality"], NULL);
  if( cparameters_.find("bitrate") != cparameters_.end() )
    g_object_set (G_OBJECT(encode_), "bitrate", cparameters_["bitrate"], NULL);

  gst_bin_add_many (GST_BIN (file_encode_), source_, colorspace_, encode_, mux_, sink_, NULL);
  gst_element_link_many (source_, colorspace_, encode_, mux_, sink_, NULL);

  return true;
}

bool FileWriteGst::setupMpeg4Pipeline(const string &filename)
{
  file_encode_ = gst_pipeline_new( "file_encode_");

  source_ = gst_element_factory_make ("appsrc", "source_");
  g_assert(source_);
  colorspace_ = gst_element_factory_make ("ffmpegcolorspace", "colorspace_");
  g_assert(colorspace_);
  encode_ = gst_element_factory_make ("ffenc_mpeg4", "encode_");
  g_assert(encode_);
  mux_ = gst_element_factory_make("avimux", "mux_");
  g_assert(mux_);
  sink_ = gst_element_factory_make ("filesink", "sink_");
  g_assert(sink_);

  g_object_set (G_OBJECT(sink_), "location", filename.c_str(), NULL);

  // set mpeg4 parameter
  if( cparameters_.find("bitrate") != cparameters_.end() )
    g_object_set (G_OBJECT(encode_), "bitrate", cparameters_["bitrate"], NULL);

  gst_bin_add_many (GST_BIN (file_encode_), source_, colorspace_, encode_, mux_, sink_, NULL);
  gst_element_link_many (source_, colorspace_, encode_, mux_, sink_, NULL);

  return true;
}

bool FileWriteGst::setupUdpPipeline(const string &filename)
{
  file_encode_ = gst_pipeline_new( "file_encode_");

  source_ = gst_element_factory_make ("appsrc", "source_");
  g_assert(source_);
  colorspace_ = gst_element_factory_make ("ffmpegcolorspace", "colorspace_");
  g_assert(colorspace_);
  encode_ = gst_element_factory_make ("ffenc_mpeg2video", "encode_");
  g_assert(encode_);
  parse_ = gst_element_factory_make ("mpegparse", "parse_");
  g_assert(parse_);
  sink_ = gst_element_factory_make ("udpsink", "sink_");
  g_assert(sink_);


  gst_bin_add_many (GST_BIN (file_encode_), source_, colorspace_, encode_, parse_, sink_, NULL);
  gst_element_link_many (source_, colorspace_, encode_, parse_, sink_, NULL);

  return true;
}

void FileWriteGst::initGstreamer()
{
  if(is_initialized_) return;

  gst_init(NULL,NULL);
  is_initialized_=true;
  
  setlocale(LC_NUMERIC, "C"); 
}

void FileWriteGst::freePipeline()
{
  if(!have_pipeline_) return;

  // Gstreamer clean up
  gst_element_set_state (file_encode_, GST_STATE_NULL);
  gst_object_unref (GST_OBJECT (file_encode_));
  have_pipeline_ = false;
}

void FileWriteGst::freeRecBuffer(void *data)
{
  if(data) delete[] (unsigned char*)data;
}


/// Tells us to register our functionality to an engine kernel
void registerPlugin(VIOKernel &K)
{
  K.getFileWriteServer().addPlugin(
    auto_ptr<FileWrite>(new FileWriteGst()));
  
  post("VideoIO: registerd FileWriteGst Plugin");
}
