//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The base class of the FileRead plugins.
//
//   FileRead
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

#include "FileRead.h"

namespace VideoIO_
{
  
  
  FileRead::FileRead() :
    auto_increment_(0), cs_format_(0),
    nr_of_frames_(0), req_frame_(0), cur_frame_(0),
    nr_of_tracks_(0), req_track_(0), cur_track_(0)
  {
  }
  

}
    
