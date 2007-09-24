//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   A dummy file read plugin.
//
//   FRDummy
//   implementation file
//
//   copyright            : (C) 2007 by Thomas Holzmann, Georg Holzmann
//   email                : holzi1@gmx.at, grh _at_ mut _dot_ at
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
///////////////////////////////////////////////////////////////////////////

#include "FileReadDummy.h"

bool FileReadDummy::openFile(string filename)
{
  duration_ = 5; // just to look if bang works
  
  frame_.allocate(20, 20, RGB);

 // has_video_file_ = true;
  
  return true;
}

unsigned char *FileReadDummy::getFrameData()
{
  int size = frame_.getXSize() * frame_.getYSize() * frame_.getColorSize();
  unsigned char *data = frame_.getFrameData();
  
  while(size--)
    *data++ = (unsigned char) rand() % 256;

  return frame_.getFrameData();
}

  /// Tells us to register our functionality to an engine kernel
void registerPlugin(VIOKernel &K)
{
  K.getFileReadServer().addPlugin(
    auto_ptr<FileRead>(new FileReadDummy()));
  
  post("VideoIO: registered FileReadDummy Plugin");
}

