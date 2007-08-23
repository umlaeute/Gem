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

#ifndef FILE_WRITE_GST_
#define FILE_WRITE_GST_

#include "../FileWrite.h"
#include "../VIOKernel.h"
#include "gst/gst.h"
#include "gst/app/gstappsrc.h"
#include "gst/app/gstappbuffer.h"

#include <string>

using namespace std;
using namespace VideoIO_;

class FileWriteGst : public FileWrite
{
 public:
  
   FileWriteGst();
  
  ~FileWriteGst();

  /*!
   * writes one frame in the video file
   * @param frame written in video file
   */
  virtual void pushFrame(VIOFrame &frame);
  
  /*!
   * opens the file at the given path
   * @param filename the path of the file
   * @return true if open worked
   */
  virtual bool openFile(const string &filename);

  /*!
   * stops recording
   * @return false if file was written
   */
  virtual bool stopRecording();

  /// prints the avaliable codecs
  virtual void getCodec();

 protected:
  GstElement *source_;
  GstElement *videorate_;
  GstElement *colorspace_; 
  GstElement *encode_;
  GstElement *mux_;
  GstElement *queue_;
  GstElement *sink_;
  GstElement *file_encode_;
  GstBus *bus_;

  /// inits video file
  void initRecording(int xsize, int ysize, int cs);

  bool setupRawPipeline(const string &filename);
  bool setupOggPipeline(const string &filename);
  bool setupMpeg4Pipeline(const string &filename);
  bool setupUdpPipeline(const string &filename);
  void freePipeline();

  bool new_video_;
  bool have_pipeline_;
  
  int frame_number_;

  static void initGstreamer();
  static bool is_initialized_;

  /// callback to free our buffer
  static void freeRecBuffer(void *data);
};

/// Tells us to register our functionality to an engine kernel
extern "C" void registerPlugin(VIOKernel &K);

#endif
