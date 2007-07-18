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

void FRDummy::openFile(const string &filename)
{
  
  post("sind im open file vom dummy");
  
  frame_ = VIOFrame(20, 20, 1);
  
  unsigned char *dummy = frame_.getFrameData();
  int i = frame_.getXSize()*frame_.getYSize()*frame_.getColorspace();
  
  int x = frame_.getXSize();
  int y = frame_.getYSize();
  int c = frame_.getColorspace();
  
  post("x= %d, y= %d, c= %d",x,y,c);
  
  for (int i=0; i < x; i++)
    for (int j=0; j < y; j++)
      for (int k=0; k < c; k++)
        frame_.setPixel(i, j, k, (unsigned char) rand()%256);
    
    has_video_file_ = true;
  
  post("sind wieder raus aus der dll");
}

  /// Tells us to register our functionality to an engine kernel
void registerPlugin(VIOKernel &K)
{
  post("hurra - wir sind in der Library da !!!!");
  
  K.getFileReadServer().addFileReadPlugin(
    auto_ptr<FileRead>(new FRDummy()));
  
  post("und schon wieder draußen");
}

