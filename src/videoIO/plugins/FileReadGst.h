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

#ifndef FILE_READ_GST_
#define FILE_READ_GST_

#include "../FileRead.h"
#include "../VIOKernel.h"
#include "gst/gst.h"
#include "gst/app/gstappsink.h"
#include "gst/app/gstappbuffer.h"

#include <string>

using namespace std;
using namespace VideoIO_;

/*!
 * \class FileReadGst
 *
 * file reader with gstreamer
 * NOTE you can debug gstreamer based applications
 * with "GST_DEBUG=3 pd -lib Gem ..."
 */
class FileReadGst : public FileRead
{
 public:
  FileReadGst();
  
  ~FileReadGst();

  /*!
   * opens the file at the given path
   * @param filename the path of the file
   * @return true if open worked
   */
  virtual bool openFile(string filename);
  
  /*!
   * closes the file
   */
  virtual void closeFile();

  /*!
   * starts playing the video asynchronus to pd
   */
  virtual void startVideo();

  /*!
   * stops playing the video
   */
  virtual void stopVideo();

  /*!
   * changes the current frame
   * you can select the frame number and the track number,
   * if track is -1 that means the same track as before
   * @param frame the number of the desired frame
   * @param track the number of the desired track
   * @return false if the frame/track does not exist
   */
  virtual bool setPosition(int frame, int track = -1);

  /// @return the frame data of VIOFrame
  virtual unsigned char *getFrameData();

 protected:
  GstElement *source_;
  GstElement *decode_;
  GstElement *colorspace_;
  GstElement *sink_;
  GstElement *file_decode_;
  GstElement *video_bin_;
  GstBus *bus_;

  bool have_pipeline_;
  bool playing_;
  bool new_video_;

  static void initGstreamer();
  static bool is_initialized_;
    
  static void cbNewpad(GstElement *decodebin, GstPad *pad, gboolean last, gpointer data);
};

/// Tells us to register our functionality to an engine kernel
extern "C" void registerPlugin(VIOKernel &K);

#endif
