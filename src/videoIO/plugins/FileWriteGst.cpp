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

FileWriteGst::FileWriteGst() : new_video_(false), have_pipeline_(false)
{

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


  /// TODO einmal weniger kopieren mache (im Prinzip braucht man
  /// den m_frame ja gar net, könnte gleich Datenpointer und
  /// Groesze übergeben

  int cs = frame.getColorSize();
  int xs = frame.getXSize();
  int ys = frame.getYSize();
  int size = xs * ys * cs;

//   post("cs: %d, size: %d", cs, size);

  unsigned char *data = frame.getFrameData();
  unsigned char *rec_data = new unsigned char[size];
  unsigned char *tmp = rec_data;

  // copy data
  for(int x=0; x<xs; ++x) {
  for(int y=0; y<ys; ++y) {
  for(int c=0; c<cs; ++c) {
    // swap y axis
    tmp[y*xs*cs + x*cs + c] =
    data[(ys-y-1)*xs*cs + x*cs + c];
  } } }

  GstBuffer *buf;
  buf = gst_app_buffer_new (rec_data, size, freeRecBuffer, (void*)rec_data);
  gst_app_src_push_buffer (GST_APP_SRC (source_), buf);
}

bool FileWriteGst::openFile(string filename)
{
  initGstreamer();

  if(have_pipeline_)
    freePipeline();

  // Test-Pipeline for gst-launch:
  // gst-launch filesrc location=input.avi ! decodebin !
  // ffmpegcolorspace ! theoraenc ! oggmux ! filesink location=output.ogg

  // setup pipeline
  file_encode_ = gst_pipeline_new( "file_encode_");

  source_ = gst_element_factory_make ("appsrc", "source_");
  g_assert(source_);
  colorspace_ = gst_element_factory_make ("ffmpegcolorspace", "colorspace_");
  g_assert(colorspace_);

  encode_ = gst_element_factory_make ("theoraenc", "encode_");
  g_assert(encode_);
  mux_ = gst_element_factory_make("oggmux", "mux_");
  g_assert(mux_);
  sink_ = gst_element_factory_make ("filesink", "sink_");
  g_assert(sink_);
  
  g_object_set (G_OBJECT(sink_), "location", filename.c_str(), NULL);

  gst_bin_add_many (GST_BIN (file_encode_), source_, colorspace_, encode_, mux_, sink_, NULL);
  gst_element_link_many (source_, colorspace_, encode_, mux_, sink_, NULL);
  
  // set ready state
  if(!gst_element_set_state (file_encode_, GST_STATE_READY))
  {
    post("The state could not be set to ready.");
    return false;
  }

  have_pipeline_=true;
  new_video_=true;
  return true;
}

bool FileWriteGst::stopRecording()
{
  if(!have_pipeline_) return false;

  gst_app_src_end_of_stream ( GST_APP_SRC(source_) );
  gst_element_set_state (file_encode_, GST_STATE_NULL);

  post("FileWriteGst: stopped recording");
  
  return true;
}

void FileWriteGst::initRecording(int xsize, int ysize, int cs)
{
  /// TODO framerate richtig umrechnen
  int fr1 = (int) framerate_;
  int fr2 = 1;

  /// TODO add endianness for OSX
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

    case RGB:
      gst_app_src_set_caps ( GST_APP_SRC(source_),
                       gst_caps_new_simple ("video/x-raw-rgb",
				     "width", G_TYPE_INT, xsize,
				     "height", G_TYPE_INT, ysize,
				     "bpp", G_TYPE_INT, 24,
				     "depth", G_TYPE_INT, 24,
				     "red_mask",   G_TYPE_INT, 16711680,
				     "green_mask", G_TYPE_INT, 65280,
				     "blue_mask",  G_TYPE_INT, 255,
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
				     "red_mask",   G_TYPE_INT, -16777216,
				     "green_mask", G_TYPE_INT, 16711680,
				     "blue_mask",  G_TYPE_INT, 65280,
				     "alpha_mask", G_TYPE_INT, 255,
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

void FileWriteGst::initGstreamer()
{
  if(is_initialized_) return;

  gst_init(NULL,NULL);
  is_initialized_=true;
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
