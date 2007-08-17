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

DeviceReadGst::DeviceReadGst() : 
    source_(NULL), demux_(NULL), decode_(NULL), colorspace_(NULL),
    sink_(NULL), device_decode_(NULL), have_pipeline_(false),new_device_(false)
{}

DeviceReadGst::~DeviceReadGst()
{
  if(have_pipeline_)
    freePipeline();
}

bool DeviceReadGst::openDevice(const string &name, const string &device)
{
  initGstreamer();
  closeDevice();

  if(name == "dv" || name == "DV")
  {
    post("opening DV device");
    setupDVPipeline();
  }
  else if( name == "video" || name == "VIDEO" )
  {
    post("opening VIDEO device");
    setupV4LPipeline(device);
  }
  else
  {
    post("unknown input device");
    return false;
  }
  
  // set READY state
  if(!gst_element_set_state (device_decode_, GST_STATE_READY))
  {
    post("The state could not be set to READY");
    return false;
  }

  new_device_ = true;
  return true;
}

void DeviceReadGst::startDevice()
{
  if(!gst_element_set_state (device_decode_, GST_STATE_PLAYING))
    post("could not set state playing");
}

void DeviceReadGst::stopDevice()
{
  if(!gst_element_set_state (device_decode_, GST_STATE_PAUSED))
    post("could not set state paused");
}

bool DeviceReadGst::closeDevice()
{
  if(have_pipeline_)
    freePipeline();

  return true;
}

unsigned char *DeviceReadGst::getFrameData()
{
  if(!have_pipeline_) 
    return 0;

  if( gst_app_sink_end_of_stream(GST_APP_SINK (sink_)) ) 
    return 0;

//   post("GST_STATE: %d, GST_STATE_PENDING: %d",
//         GST_STATE(device_decode_), GST_STATE_PENDING(device_decode_));

  GstBuffer *buf = 0;
  
  if( GST_STATE(device_decode_)==GST_STATE_PLAYING &&
      GST_STATE_PENDING(device_decode_)==GST_STATE_VOID_PENDING )
  {
    buf = gst_app_sink_pull_buffer(GST_APP_SINK (sink_));
  }

  if( !buf ) return 0;

  guint8 *data = GST_BUFFER_DATA( buf );
  guint size = GST_BUFFER_SIZE( buf );

  if( new_device_ )
  {
    GstCaps *caps = gst_buffer_get_caps (buf);
    GstStructure *str = gst_caps_get_structure (caps, 0);

    post("DeviceReadGst opened device: %s",
          gst_caps_to_string (caps) );

    // getting fomrat options
    int x_size, y_size, bpp, depth;
    int fps_numerator, fps_denominator;
    g_assert( gst_structure_get_int(str, "width", &x_size) );
    g_assert( gst_structure_get_int(str, "height", &y_size) );

    int format=-1;
    gst_structure_get_int(str, "bpp", &bpp);
    gst_structure_get_int(str, "depth", &depth);
    format = YUV422; // for YUV there's no bpp and depth
    if( bpp==24 && depth==24 ) format=RGB;
    if( bpp==8 && depth==8 ) format=GRAY;
    if( bpp==32 ) format=RGBA;

    // allocate frame
    frame_.allocate(x_size, y_size, format);

    new_device_=false;
    gst_caps_unref(caps);
  }
    
  unsigned char *frame = frame_.getFrameData();

  int xs = frame_.getXSize();
  int ys = frame_.getYSize();
  int cs = frame_.getColorSize();

//   post("xs: %d, ys: %d, cs: %d", xs, ys, cs);

  for(int x=0; x<xs; ++x) {
  for(int y=0; y<ys; ++y) {
  for(int c=0; c<cs; ++c) {
    // swap y axis
    frame[y*xs*cs + x*cs + c] =
    data[(ys-y-1)*xs*cs + x*cs + c];
  } } }

  gst_buffer_unref (buf);
  
  return frame_.getFrameData();
}

void DeviceReadGst::cbNewpad(GstElement *element, GstPad *pad, gpointer data)
{
  GstCaps *caps;
  GstStructure *str;
  GstPad *videopad;

  DeviceReadGst *tmp = (DeviceReadGst *) data;

  // only link once
  videopad = gst_element_get_pad (tmp->decode_, "sink");
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
				"red_mask",   G_TYPE_INT, 0xff000000,
				"green_mask", G_TYPE_INT, 0x00ff0000,
				"blue_mask",  G_TYPE_INT, 0x0000ff00,
				"alpha_mask", G_TYPE_INT, 0x000000ff,
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

void DeviceReadGst::cbFrameDropped(GstElement *element, gpointer data)
{
  post("FileReadGst: dropped frame");
}

void DeviceReadGst::initGstreamer()
{
  if(is_initialized_) return;

  gst_init(NULL,NULL);
  is_initialized_=true;
}

void DeviceReadGst::setupDVPipeline()
{
  /// Test-pipeline for DV with GStreamer:
  /// gst-launch dv1394src ! dvdemux ! dvdec ! ffmpegcolorspace !
  /// theoraenc ! oggmux ! filesink location=test.ogg

  device_decode_ = gst_pipeline_new( "device_decode_");

  source_ = gst_element_factory_make ("dv1394src", "source_");
  g_assert(source_);
  g_object_set (G_OBJECT(source_), "use-avc", false, NULL);
  /// TODO control of the DV camera would also be quite easy
  /// to implement: set "use-avc" to true
  /// then GST_STATE_PLAYING starts and GST_STATE_READY stops
  /// the camera, seeking is done with usual gstreamer seeking
  g_signal_connect (source_, "frame-dropped", G_CALLBACK (cbFrameDropped), (gpointer)this);

  demux_ = gst_element_factory_make ("dvdemux", "demux_");
  g_assert(demux_);

  decode_ = gst_element_factory_make ("dvdec", "decode_");
  g_assert(decode_);
  g_object_set (G_OBJECT(decode_), "quality", dv_quality_, NULL);

  colorspace_ = gst_element_factory_make ("ffmpegcolorspace", "colorspace_");
  g_assert(colorspace_);
  sink_ = gst_element_factory_make ("appsink", "sink_");
  g_assert(sink_);
  
  gst_bin_add_many (GST_BIN(device_decode_), source_, demux_, decode_, colorspace_, sink_, NULL);
  
  gst_element_link (source_, demux_);
  gst_element_link (decode_, colorspace_);
  
  g_signal_connect (demux_, "pad-added", G_CALLBACK (cbNewpad), (gpointer)this);
  
  have_pipeline_ = true;
}

void DeviceReadGst::setupV4LPipeline(const string &device)
{
  /// Test-pipeline for v4l with GStreamer:
  /// gst-launch v4lsrc ! ffmpegcolorspace !
  /// theoraenc ! oggmux ! filesink location=test.ogg

  device_decode_ = gst_pipeline_new( "device_decode_");

  source_ = gst_element_factory_make ("v4lsrc", "source_");
  g_assert(source_);
  if( device.size() != 0 )
    g_object_set (G_OBJECT(source_), "device", device.c_str(), NULL);

  colorspace_ = gst_element_factory_make ("ffmpegcolorspace", "colorspace_");
  g_assert(colorspace_);
  sink_ = gst_element_factory_make ("appsink", "sink_");
  g_assert(sink_);

  gst_bin_add_many (GST_BIN(device_decode_), source_, colorspace_, sink_, NULL);
  gst_element_link (source_, colorspace_);

  // force a colorspace conversion if requested
  switch( cspace_ )
  {
    case RGBA:
      gst_element_link_filtered(colorspace_, sink_,
           gst_caps_new_simple ("video/x-raw-rgb", 
                                "bpp", G_TYPE_INT, 32,
                                "depth", G_TYPE_INT, 32,
				"red_mask",   G_TYPE_INT, 0xff000000,
				"green_mask", G_TYPE_INT, 0x00ff0000,
				"blue_mask",  G_TYPE_INT, 0x0000ff00,
				"alpha_mask", G_TYPE_INT, 0x000000ff,
                                NULL) );
      break;

    case RGB:
      gst_element_link_filtered(colorspace_, sink_,
           gst_caps_new_simple ("video/x-raw-rgb", 
                                "bpp", G_TYPE_INT, 24,
                                "depth", G_TYPE_INT, 24,
				"red_mask",   G_TYPE_INT, 0x00ff0000,
				"green_mask", G_TYPE_INT, 0x0000ff00,
				"blue_mask",  G_TYPE_INT, 0x000000ff,
                                NULL) );
      break;

    case GRAY:
      gst_element_link_filtered(colorspace_, sink_,
           gst_caps_new_simple ("video/x-raw-gray", NULL) );
      break;

    // also set default to the GEM YUV format, because
    // usually CAMs use a different YUV format
    default:
    case YUV422:
      gst_element_link_filtered(colorspace_, sink_,
           gst_caps_new_simple ("video/x-raw-yuv", 
                                "format", GST_TYPE_FOURCC,
                                GST_MAKE_FOURCC('U', 'Y', 'V', 'Y'),
                                NULL) );
      break;
  }

  have_pipeline_ = true;
}

void DeviceReadGst::freePipeline()
{
  if(!have_pipeline_) return;

  // Gstreamer clean up
  gst_element_set_state (device_decode_, GST_STATE_NULL);
  gst_object_unref (GST_OBJECT (device_decode_));
  have_pipeline_ = false;
}


/// Tells us to register our functionality to an engine kernel
void registerPlugin(VIOKernel &K)
{
  K.getDeviceReadServer().addPlugin(
    auto_ptr<DeviceRead>(new DeviceReadGst()) );
  
  post("VideoIO: registerd DeviceReadGst Plugin");
}
