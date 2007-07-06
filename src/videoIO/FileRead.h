//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   Loads a digital video (like AVI, Mpeg, Quicktime) into a VIOFrame.
//
//   FileRead
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

    
#infdef INCLUDE_FILEREAD_H_
#define INCLUDE_FILEREAD_H_

#include "VIOUtils.h"

class VIOFrame;
class FReadPlugin;

/*!
 * \class FileRead
 * 
 * \brief class for reading files
 * 
 * This class loads the plugins needed for decoding and ...
*/
class FileRead
{
  public:
    
  /// constructor
  FileRead();
  
  /// destructor
  virtual ~FileRead();
  
  /*!
   * opens the file at the given path
   * @param filename the path of the file
   */
  void openFile(t_symbol *filename);
  
  /*!
   * closes the file
   */
  void closeFile();
  
  /*!
   * @return the current frame of the video
   */
  VIOFrame *getFrame();
  
  /*!
   * changes which frame to display
   * you can select the frame number and the track number,
   * if track is -1 that means the same track as before
   * @param frame the number of the desired frame
   * @param track the number of the desired track
   */
  void setPosition(int frame, int track = -1);
  
  /*!
   * if this function called, getFrame will automatically
   * increment the frame number with incr
   * @param incr the desired incrementation value
   */
  void setAutoIncrement(t_float incr);
  
  
  
  //////////////////////
  // Utility methods
  /////////////////////
  
  
  /*!
   * @return the number of frames
   */
  int getNrOfFrames ();
  
  /*!
   * @return the frames per second
   */
  double getFPS();
  
  /*!
   * @return the width of the video
   */
  int getWidth();
  
  /*!
   * @return the height of the video
   */
  int getHeight();
  
  /*!
   * @return true if a video is loaded
   */
  bool hasVideo();
  
  
  protected:
  
  bool hasVideo;
  t_float autoIncrement;
  int csFormat;
  
  // frame information
  int nrOfFrames;
  t_float reqFrame;
  int curFrame;
  
  // track information
  int nrOfTracks
  int reqTrack
  int curTrack

  /// the video file
  FReadPlugin *video;
  
  /// stores the current frame
  VIOFrame *frame;
};

#endif
  
  