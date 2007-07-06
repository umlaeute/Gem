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


class VIOFrame
{
  
  VIOFrame(x_size, y_size, colorspace);
  VIOFrame();
  
  ~VIOFrame();
  
  void allocate(x_size, y_size, colorspace);
  
  inline unsigned char getPixel(int x, int y, int color)
  { return(data[x * xsize * csize + y * csize + color]); }
  
  inline void setPixel(int x, int y, int color, unsigned char value)
  { data[x * xsize * csize + y * csize + color] = value; }
  
  int getXSize();
  int getYSize();
  int getColorspace();
  
  
 protected:
  
  unsigned char *data;
  
  int x_size;
  int y_size;
  int colorspace;
  
};

#endif
