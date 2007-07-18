//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The Frame class
//
//   VIOFrame
//   implementation file
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

#include "VIOFrame.h"


#include "VIOUtils.h"


namespace VideoIO_
{
  VIOFrame::VIOFrame(int x_size, int y_size, int colorspace) :
      data_(0)
  {
    allocate(x_size, y_size, colorspace);
  }
  
  VIOFrame::VIOFrame() : x_size_(0), y_size_(0), colorspace_(0),
                     data_(0)
  {
    allocate(x_size_, y_size_, colorspace_);
  } 
  
  VIOFrame::~VIOFrame()
  {
    if(data_) delete [] data_;
    data_=0;
  }
  
  void VIOFrame::allocate(int x_size, int y_size, int colorspace)
  {
    if(data_) delete [] data_;
    data_=0;
    
    x_size_ = x_size;
    y_size_ = y_size;
    colorspace_ = colorspace;
    
    data_ = new unsigned char[x_size*y_size*colorspace];
  }
}
