//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   A dummy file read plugin.
//
//   FRDummy
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

#include "FRDummy.h"

bool FRDummy::openFile(string filename)
{
  nr_of_frames_ = 5; // just to look if bang works
  
  frame_.allocate(20, 20, RGB);

  has_video_file_ = true;
  
  return true;
}

unsigned char *FRDummy::getFrameData()
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
    auto_ptr<FileRead>(new FRDummy()));
  
  post("VideoIO: registered Dummy FileRead Plugin");
}

