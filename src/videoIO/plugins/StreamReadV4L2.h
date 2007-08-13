//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   Plugin for Video 4 Linux 2
//
//   StreamReadV4L2
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


#ifndef STREAM_READ_V4L2_
#define STREAM_READ_V4L2_

#include <string>

using namespace std;

namespace VideoIO_
{
  class StreamReadV4L2 : public StreamRead
  {
    public:
    
    /// constructor
    videoV4L2();
    
    /// destructor
    ~videoV4L2();
    
    /*!
     * opens the V4L device
     * @param device_number the number of the device
     * @param format the requested format
     * @return true if the device was successfully opened
     */
    bool  openDevice(int devnum, int format=0){return 1;}
    
    /// closes the device
    void  closeDevice(void){}
    
    /*!
     * resets the device
     * @return true if the device was successfully reset
     */
    bool resetDevice(void){return 1;}
      
    /*!
     * Start up the video device (called on startRendering)
     * @param format the requested format
     * @return false if transfer doesn't work
     */
    bool startTransfer(int format=0);
    
    /*!
     * Stops the transfer
     * @return true if successful
     */
    bool stopTransfer();
    
    /// restarts the transfer
    void restartTransfer();
    
    /// @return the next frame
    VIOFrame *getFrame();
    
    /// sets the video dimensions
    int  setDimen(int x, int y, int leftmargin, int rightmargin,
                                  int topmargin, int bottommargin);
    int  setChannel(int c, float f);
    int  setNorm(string);
    int  setDevice(int);
    int  setDevice(string);
    int  setColor(int);
    
      
    protected:
    
    //-----------------------------------
    // GROUP:	Linux specific video data
    //-----------------------------------
          
    
    int v4l2_format_; // the format returned by the v4l2-device (not an openGL-format!)
    bool color_convert_; // do we have to convert the colour-space manually ?
    
   ///TODO do we need this? 
    //int m_tvfd;
    
    struct V4L2Buffer *buffers_;
    int  nr_of_buffers_;
    void *current_buffer_;
    
    int frame_;
    int last_frame_;
    
    //////////////////
    // capabilities of the device
    int max_width_;
    int min_width_;
    int max_height_;
    int min_height_;
    
   
    // rendering might be needed when we are currently not capturing because we cannot (e.g: couldn't open device)
    // although we should. when reopening another device, we might be able to render again...
    // example: we have only 1 video-device /dev/video0;
    // when we try to open /dev/video1 we fail, and m_capturing is set to 0
    // now when rendering is turned on and we want to switch back to /dev/video0 we should reconnect to the good device
    bool rendering_; // "true" when rendering is on, false otherwise
  };
  
  struct V4L2Buffer 
  {
    void *start_;
    size_t length_;
  };

}

#endif
