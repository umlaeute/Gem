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
  
  VIOFrame::VIOFrame() : x_size_(0), y_size_(0), color_size_(0),
                     data_(0)
  {
    allocate(x_size_, y_size_, color_size_); /// TODO alloziert nicht richtig, da hier
                                             /// colorspace und nicht color_size übergeben
                                             /// werden sollte
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
    setColorSize(colorspace);
    
    data_ = new unsigned char[x_size_*y_size_*color_size_];
  }
  
  void VIOFrame::setColorSize (int format)
  {
    switch(format)
    {
      case GRAY:  
        color_size_ = 1; 
        break;
        
      case YUV422:
        color_size_ = 2;
        break;
        
      case RGB: 
        color_size_ = 3;
        break;
    
      case RGBA:
      default:
        color_size_ = 4; 
        break;
    }
  }
}
