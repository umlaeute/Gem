//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   Writes a digital video (like AVI, Mpeg, Quicktime) to the harddisc.
//
//   FileWrite
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

#ifndef FILE_WRITE_PLUGIN_
#define FILE_WRITE_PLUGIN_

using namespace std;

#include "VIOUtils.h"
#include "VIOFrame.h"

namespace VideoIO_
{
  class FileWrite
  {
    public:
    /// constructor
    FileWrite();
    
    /// destructor
    virtual ~FileWrite(){};

    /*!
     * stops recording
     * @return false if file was written
     */
    virtual bool stopRecording() = 0;
    
    /*!
     * writes one frame in the video file
     * @param frame written in video file
     */
    virtual void pushFrame(VIOFrame &frame) = 0;
    
    /*!
     * opens the file at the given path
     * @param filename the path of the file
     * @return true if open worked
     */
    virtual bool openFile(string filename) = 0;

    /// set framerate of the video
    virtual void setFramerate(float fr);
    
    /// sets the desired codec
    void setCodec(string codec);
    
    /// prints the avaliable codecs
    virtual void getCodec();

  protected:
    
    float framerate_;
    string codec_;
  };
}

#endif
