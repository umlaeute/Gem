//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   Loads a video stream (like AVI, Mpeg, Quicktime) into a VIOFrame.
//
//   StreamRead
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

#include "StreamRead.h"


namespace VideoIO_
{
  
  StreamRead::StreamRead() : 
      capturing_(false), has_video_(false), 
      width_(0), height_(0), channel_(0), device_number_(0),
      tv_norm_(0), req_format_(0), quality_(0)
  {}
  
  StreamRead::~StreamRead()
  {
    if(has_video_)
      closeDevice();
  }
  
}
