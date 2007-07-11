//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The Frame class
//
//   vioutils
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

/*!
 * \class VIOFrame
 * 
 * This class stores the frame data and has
 * some methods needed for the frame.
 */


class VIOFrame
{
  /// constructor
  VIOFrame(x_size, y_size, colorspace);
  
  /// constructor
  VIOFrame();
  
  /// destructor
  ~VIOFrame();
  
  /*!
   * allocates the memory and then also frees
   * the memory
   * @param x_size the horizontal size of the frame
   * @param y_size the vertical size of the frame
   * @param colorspace the colorspace of the frame
   */
  void allocate(x_size, y_size, colorspace);
  
  /*!
   * @param x the x coordinate
   * @param y the y coordinate
   * @param color the colorspace
   * @return the pixel data
   */
  inline unsigned char getPixel(int x, int y, int color)
  { return(data[x * xsize * csize + y * csize + color]); }
  
  /*!
   * sets a pixel
   * @param x the x coordinate
   * @param y the y coordinate
   * @param color the colorspace
   * @param value the value to set
   */
  inline void setPixel(int x, int y, int color, unsigned char value)
  { data[x * xsize * csize + y * csize + color] = value; }
  
  /// @return the x size
  int getXSize();
  /// @return the y size
  int getYSize();
  /// @return the colorspace
  int getColorspace();
  
  
 protected:
  
  /// the frame data
  unsigned char *data;
  
  int x_size;
  int y_size;
  int colorspace;
  
};

#endif
