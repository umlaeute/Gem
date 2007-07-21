//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The base class of the FileRead plugins.
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

    
#ifndef FILE_READ_PLUGIN_
#define FILE_READ_PLUGIN_

using namespace std;

#include "VIOUtils.h"
#include "VIOFrame.h"

namespace VideoIO_
{
  class FileRead
  {
    public:
    /// constructor
    FileRead();
    
    /// destructor
    virtual ~FileRead(){};
    
    /*!
    * opens the file at the given path
    * @param filename the path of the file
    * @return true if open worked
    */
    virtual bool openFile(char *filename) = 0; ///TODO string?? 
    
    /*!
    * closes the file
    */
    virtual void closeFile() {
      /// TODO das implementieren bitte bzw in den abgeleiteten klassen !
      post("closeFile in FileRead");
    };

    /// @return the frame data of VIOFrame
    inline virtual unsigned char *getFrameData() {return frame_.getFrameData();}
  
    /*!
    * changes which frame to display
    * you can select the frame number and the track number,
    * if track is -1 that means the same track as before
    * @param frame the number of the desired frame
    * @param track the number of the desired track
    * @return true if this is the last frame
    */
    virtual bool setPosition(int frame, int track = -1);
    
    /*!
    * if this function called, getFrame will automatically
    * increment the frame number with incr
    * @param incr the desired incrementation value
    */
    virtual void setAutoIncrement(t_float incr) {};
    
    /// TODO mit der auto variable richtig handlen, d.h. bei getFrame MUSS
    ///      immer auto auch erhöht werde (wenn aktiviert) !!!!!
    
      //////////////////////
    // Utility methods
    /////////////////////
    
    /*!
    * @return the number of frames
    */
    virtual int getNrOfFrames () = 0;
    
    /*!
    * @return the frames per second
    */
    virtual double getFPS() = 0;
    
    /*!
    * @return the width of the video
    */
    virtual int getWidth() = 0;
    
    /*!
    * @return the height of the video
    */
    virtual int getHeight() = 0;
    
    /*!
    * @return true if a video is loaded
    */
    bool hasVideo() {return has_video_file_;};
  
    protected:
    
      bool has_video_file_;
      t_float auto_increment_;
      int cs_format_;
    
    // frame information
      int nr_of_frames_;
      int req_frame_;
      int cur_frame_;
    
    // track information
      int nr_of_tracks_;
      int req_track_;
      int cur_track_;
  
    /// the video file
      //FReadPlugin *video;
    
    /// stores the current frame
      VIOFrame frame_ ;
    
    
  };
}
#endif
