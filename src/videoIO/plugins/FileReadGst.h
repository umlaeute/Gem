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

class FileReadGst : public FileRead
{
  public:
  FileReadGst();
  
  ~FileReadGst();
  
  bool openFile(string filename);
  
  /// @return the frame data of VIOFrame
  virtual unsigned char *getFrameData();
  
  /*!
  * @return the number of frames
  */
  inline int getNrOfFrames () {return 1;};
  
  /*!
  * @return the frames per second
  */
  inline double getFPS() {return 2;};
  
  /*!
  * @return the width of the video
  */
  inline int getWidth() {return x_size_;};
  
  /*!
  * @return the height of the video
  */
  inline int getHeight() {return y_size_;};
  
  void setVideoProperties( int x_size, int y_size, double fps);
  
  protected:
    GstElement *source_;
    GstElement *decode_;
    GstElement *colorspace_;
    GstElement *sink_;
    GstElement *file_decode_;
    GstElement *video_bin_;
    GstBus *bus_;
    
    int x_size_;
    int y_size_;
    double fps_;
    

    static bool is_initialized_;
    
    static void cbNewpad(GstElement *decodebin, GstPad *pad, gboolean last, gpointer data);
};


/// Tells us to register our functionality to an engine kernel
extern "C" void registerPlugin(VIOKernel &K);

#endif
