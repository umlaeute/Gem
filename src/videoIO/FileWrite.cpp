//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   Writes a digital video (like AVI, Mpeg, Quicktime) to the harddisc.
//
//   FileWrite
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

#include "FileWrite.h"

namespace VideoIO_
{

  FileWrite::FileWrite() :
    x_size_(0), y_size_(0)
  {}
  
  void FileWrite::setSize(int width, int height)
  {
    /// TODO range auf richtigen Bereich abchecken (keine minus oder riesigen zahlen)
    x_size_ = width;
    y_size_ = height;
  }

  void FileWrite::setColorspace(int cs)
  {
    cspace_ = cs;
  }
}
