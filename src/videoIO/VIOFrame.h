//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The Frame class
//
//   VIOFrame
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
    
#ifndef VIOFRAME_H_
#define VIOFRAME_H_

using namespace std;

#include <memory>

namespace VideoIO_
{
  class VIOUtils;
  
  /*!
  * \class VIOFrame
  * 
  * This class stores the frame data and has
  * some methods needed for the frame.
  */
  class VIOFrame
  {
    public:
    /// constructor
    VIOFrame(int x_size, int y_size, int colorspace);
    
    /// constructor
    VIOFrame();
    
    /// destructor
    virtual ~VIOFrame();
    
    /*!
    * allocates the memory and then also frees
    * the memory
    * @param x_size the horizontal size of the frame
    * @param y_size the vertical size of the frame
    * @param colorspace the colorspace of the frame
    */
    void allocate(int x_size, int y_size, int colorspace);
    
    /*!
    * @param x the x coordinate
    * @param y the y coordinate
    * @param color the colorspace
    * @return the pixel data
    */
    inline unsigned char getPixel(int x, int y, int color)
    { return data_[x * x_size_ * colorspace_ + y * colorspace_ + color]; }
    
    /*!
    * sets a pixel
    * @param x the x coordinate
    * @param y the y coordinate
    * @param color the colorspace
    * @param value the value to set
    */
    inline void setPixel(int x, int y, int color, unsigned char value)
    { data_[x * x_size_ * colorspace_ + y * colorspace_ + color] = value; }
    
    /// @return the x size
    inline int getXSize()
    { return x_size_; }
    
    /// @return the y size
    inline int getYSize()
    { return y_size_; }
    
    /// @return the colorspace
    inline int getColorspace()
    { return colorspace_; }
    
    /// NOTE data_ must be deleted by the client afterwards
    ///      VIOFrame won't free the memory !!!
    /// @return a pointer to the frame data
    inline unsigned char *getFrameData()
    { 
      free_data_ = false;
      return data_;
    }
    
    
  protected:
    
    /// the frame data
    unsigned char *data_;
    
    /// is false if we are not responsible to free data_
    bool free_data_;
    
    int x_size_;
    int y_size_;
    int colorspace_;
    
  };
}

#endif
