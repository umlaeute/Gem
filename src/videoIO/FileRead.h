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
    * changes the position in the video
    * @param sec desired position in the stream in sec
    * @return false if there was a problem
    */
    virtual bool setPosition(float sec) = 0;
    
    /*!
     * sets the playing speed of the video
     * @param speed the desired speed (e.g. 2.0 for double speed)
     */
    virtual void setSpeed(float speed) = 0;

    /*!
    * force a specific colorspace
    * normally the native colorspace of the movie is used,
    * here one can force a conversion to an other colorspace
    * @param cs one of the colorspace defines, if this is
    *           < 0 then the native movie format will be used
    */
    virtual void forceColorspace(int cs)
    { cspace_ = cs; }

    /// set framerate of the host application (PD)
    virtual void setHostFramerate(float fr)
    { fr_host_ = fr; }
    
    virtual void setAudioTrack(int track)
    { atrack_ = track; }

    virtual void setVideoTrack(int track)
    { vtrack_ = track; } 
   
    /*!
     * returns the data of the current frame
     * @return the frame data of VIOFrame
     */
    inline virtual unsigned char *getFrameData()
    { return frame_.getFrameData(); }

    /*!
     * writes stereo audio data for one block to pointers
     * each pointer is responsible to allocate memory of
     * size n before !
     * @param left pointer to channel left audio samples
     * @param right pointer to channel right audio samples
     * @param n blocksize, nr of sample to grab for each channel
     */
    virtual void getAudioBlock(t_float *left, t_float *right, int n) = 0;

    //////////////////////
    // Utility methods
    /////////////////////
    
    /*!
    * @return duration of the movie in sec
    */
    virtual float getDuration()
    { return duration_; }
    
    /*!
    * @return framerate of loaded video
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
    // the desired videotrack
    int vtrack_;
    //the desired audiotrack
    int atrack_;
    
    // frame information
    float duration_;
    float framerate_;
    float fr_host_;
    
    /// stores the current frame
    VIOFrame frame_ ;
  };
}
#endif
