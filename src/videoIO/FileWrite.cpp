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

// NOTE: don't remove this (small) cpp file, because
// we need an object generated out of the cpp

namespace VideoIO_
{

  FileWrite::FileWrite() :
    framerate_(1)
  {}

  void FileWrite::setFramerate(float fr)
  {
    framerate_ = fr;
  }
  
  void FileWrite::setCodec(const string &codec)
  {
    codec_ = codec;
  }
  
  void FileWrite::getCodec()
  {
    post("videoIO: Sorry, no plugins for file writing avaliable");
  }
}
