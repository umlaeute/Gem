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
    FileRead() : cspace_(-1), nr_of_frames_(0), framerate_(0)
    {};
    
    /// destructor
    virtual ~FileRead(){};
    
    /*!
    * opens the file at the given path
    * @param filename the path of the file
    * @return true if open worked
    */
    virtual bool openFile(string filename) = 0;
    
    /*!
    * closes the file
    */
    virtual void closeFile() = 0;

    /*!
    * starts playing the video asynchronus to pd
    */
    virtual void startVideo() = 0;

    /*!
    * stops playing the video
    */
    virtual void stopVideo() = 0;

    /*!
    * changes the current frame
    * you can select the frame number and the track number,
    * if track is -1 that means the same track as before
    * @param frame the number of the desired frame
    * @param track the number of the desired track
    * @return false if the frame/track does not exist
    */
    virtual bool setPosition(int frame, int track = -1);

    /*!
    * force a specific colorspace
    * normally the native colorspace of the movie is used,
    * here one can force a conversion to an other colorspace
    * @param cs one of the colorspace defines, if this is
    *           < 0 then the native movie format will be used
    */
    virtual void forceColorspace(int cs)
    { cspace_ = cs; }

    /*!
     * returns the data of the current frame
     * @return the frame data of VIOFrame
     */
    inline virtual unsigned char *getFrameData()
    { return frame_.getFrameData(); }

    //////////////////////
    // Utility methods
    /////////////////////
    
    /*!
    * @return the number of frames
    */
    virtual int getNrOfFrames()
    { return nr_of_frames_; }
    
    /*!
    * @return the frames per second
    */
    virtual double getFPS()
    { return framerate_; }
    
    /*!
    * @return the width of the video
    */
    virtual int getWidth()
    { return frame_.getXSize(); }
    
    /*!
    * @return the height of the video
    */
    virtual int getHeight()
    { return frame_.getYSize(); }

    /*!
    * @return the colorspace
    */
    virtual int getColorspace()
    { return frame_.getColorspace(); }
    
    /*!
    * @return the colorsize in bytes
    */
    virtual int getColorSize()
    { return frame_.getColorSize(); }
  
   protected:
    
    // force a specific colorspace
    int cspace_;
    
    // frame information
    int nr_of_frames_;
    float framerate_;
    
    /// stores the current frame
    VIOFrame frame_ ;
  };
}
#endif
