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

#include <string>

using namespace std;
using namespace VideoIO_;

class FileReadGst : public FileRead
{
  public:
  FileReadGst();
  
  ~FileReadGst()
  {
      /* also clean up */
  gst_element_set_state (play_, GST_STATE_NULL);
  gst_object_unref (GST_OBJECT (play_));
  }
  
  bool openFile(string filename);
  
  unsigned char *getFrameData(){};
  
  
  /*!
  * @return the number of frames
  */
  virtual int getNrOfFrames () {};
  
  /*!
  * @return the frames per second
  */
  virtual double getFPS() {};
  
  /*!
  * @return the width of the video
  */
  virtual int getWidth() {};
  
  /*!
  * @return the height of the video
  */
  virtual int getHeight() {};
  
  protected:
    GstElement *play_;
    
    static bool is_initialized_;
  
};


/// Tells us to register our functionality to an engine kernel
extern "C" void registerPlugin(VIOKernel &K);

#endif
