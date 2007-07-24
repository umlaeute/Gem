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
    * the memory but only if needed !
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
    { return data_[x * x_size_ * color_size_ + y * color_size_ + color]; }
    
    /*!
    * sets a pixel
    * @param x the x coordinate
    * @param y the y coordinate
    * @param color the colorspace
    * @param value the value to set
    */
    inline void setPixel(int x, int y, int color, unsigned char value)
    { // printf("----------------setPixel called---------------\n");
      data_[x * x_size_ * color_size_ + y * color_size_ + color] = value; }
      
    /*!
    * sets the color_size_ variable
    * @param format the colorspace format
    */
    void setColorSize (int format);
      
    
    /// @return the x size
    inline int getXSize()
    { //printf("----------------getXSize called-------------------\n");
      return x_size_; }
    
    /// @return the y size
    inline int getYSize()
    { //printf("----------------getYSize called-------------------\n");
      return y_size_; }
    
    /// @return the colorspace
    inline int getColorSize()
    { //printf("----------------getColorspace called-------------------\n");
      return color_size_; }
    
    /// @return a pointer to the frame data
    inline unsigned char *getFrameData()
    { 
      return data_;
    }
    
    
  protected:
    
    /// the frame data
    unsigned char *data_;
    
    int x_size_;
    int y_size_;
    int color_size_; 
    
  };
}

#endif
