//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   Plugin for Video 4 Linux 2
//
//   StreamReadV4L2
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

#include "StreamReadV4L2.h"


namespace VideoIO_
{
  StreamReadV4L2::StreamReadV4L2 :
      v4l2_format_(0), color_convert_(false), buffers_(NULL),
      nr_of_buffers_(0), current_buffer_(NULL), frame_(0), 
      last_frame_(0), max_width_(0), min_width_(0),
      max_height_(0), min_height_(0), rendering_(false)
  {}
      