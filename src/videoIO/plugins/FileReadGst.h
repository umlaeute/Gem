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
#include "gst/base/gstadapter.h"

#include <string>

using namespace std;
using namespace VideoIO_;

/*!
 * \class FileReadGst
 *
 * file reader with gstreamer
 * NOTE you can debug gstreamer based applications
 * with "GST_DEBUG=3 pd -lib Gem ..."
 * to debug one single plugin, e.g.:
 * "GST_DEBUG=appsink:5 pd -lib Gem ..."
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
  bool openFile(string filename);
  
  /// creates the audio bin on demand
  /// @return true if successful
  bool createAudioBin();
  
  /*!
   * closes the file
   */
  void closeFile();

  /*!
   * starts playing the video asynchronus to pd
   */
  void startVideo();

  /*!
   * stops playing the video
   */
  void stopVideo();

  /*!
   * changes the position in the video
   * @param sec desired position in the stream in sec
   * @return false if there was a problem
   */
  bool setPosition(float sec);
  
  /*!
   * sets the playing speed of the video
   * @param speed the desired speed (e.g. 2.0 for double speed)
   */
  void setSpeed(float speed);

  /// @return the frame data of VIOFrame
  unsigned char *getFrameData();

  /*!
   * writes stereo audio data for one block to pointers
   * each pointer is responsible to allocate memory of
   * size n before !
   * @param left pointer to channel left audio samples
   * @param right pointer to channel right audio samples
   * @param n blocksize, nr of sample to grab for each channel
   */
  void getAudioBlock(t_float *left, t_float *right, int n);

 protected:
  GstElement *source_;
  GstElement *videorate_;
  GstElement *colorspace_;
  GstElement *vqueue_;
  GstElement *vsink_;
  GstElement *aconvert_;
  GstElement *aresample_;
  GstElement *aqueue_;
  GstElement *asink_;
  GstElement *file_decode_;
  GstElement *video_bin_;
  GstElement *audio_bin_;
  GstAdapter *adapter_;
  GstBus *bus_;

  bool have_pipeline_;
  bool new_video_;

  string getURIFromFilename(const string &filename);

  static void initGstreamer();
  static bool is_initialized_;
    
  static void cbNewpad(GstElement *decodebin, GstPad *pad, gpointer data);
};

/// Tells us to register our functionality to an engine kernel
extern "C" void registerPlugin(VIOKernel &K);

#endif
