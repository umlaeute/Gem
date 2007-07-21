//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The Quicktime 4 Linux file read plugin
//
//   FileReadQT4L
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

#ifndef FILE_READ_QT4L_
#define FILE_READ_QT4L_

#include "../FileRead.h"
#include "../VIOKernel.h"  /// TODO forward deklarieren !!!

#include "lqt/quicktime.h" // quicktime test
#include "lqt/colormodels.h"

#include <string>

using namespace std;
using namespace VideoIO_;

class FileReadQT4L : public FileRead
{
  public:
  /// constructor
  FileReadQT4L();
  
  
  /// destructor
  ~FileReadQT4L();
  
  /*!
  * opens the file at the given path
  * @param filename the path of the file
  * @return true if open worked
  */
  bool openFile(char *filename);
  
  /*!
  * closes the file
  */
  void closeFile();

  /*!
  * changes which frame to display
  * you can select the frame number and the track number,
  * if track is -1 that means the same track as before T
  *                TODO this doesn't mean the same in qt4linux??
  * @param frame the number of the desired frame
  * @param track the number of the desired track
  * @return true if this is the last frame
  */
  bool setPosition(int frame, int track = -1);
  
  /// @return the frame data of VIOFrame
  unsigned char *getFrameData();
  
    //////////////////////
  // Utility methods
  /////////////////////
  
  /*!
  * @return the number of frames
  */
  int getNrOfFrames () 
  {return (int) quicktime_video_length(qt_file_, cur_track_);};
  
  /*!
  * @return the frames per second
  */
  double getFPS() 
  {return quicktime_frame_rate(qt_file_, cur_track_);};
  
  /*!
  * @return the width of the video
  */
  int getWidth() 
  {return quicktime_video_width(qt_file_, cur_track_);};
  
  /*!
  * @return the height of the video
  */
  int getHeight() 
  {return quicktime_video_height(qt_file_, cur_track_);};
  
  private:
  quicktime_t *qt_file_;
  
};

/// Tells us to register our functionality to an engine kernel
extern "C" void registerPlugin(VIOKernel &K);

#endif
