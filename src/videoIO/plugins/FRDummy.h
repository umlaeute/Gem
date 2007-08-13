//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   A dummy file read plugin.
//
//   FRDummy
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


#ifndef FRDUMMY_
#define FRDUMMY_

#include "FRDummy.h"
#include "../FileRead.h"
#include "../VIOKernel.h"

#include <string>

using namespace std;
using namespace VideoIO_;

class FRDummy : public FileRead
{
  public:
  /*!
  * opens the file at the given path
  * @param filename the path of the file
  */
  bool openFile(string filename);
  
  /*!
  * @return the current frame of the video
  */
  unsigned char *getFrameData();

  /*!
  * @return the frames per second
  */
  double getFPS() { return 20.; };
  
  /*!
  * @return the width of the video
  */
  int getWidth() { return 20; };
  
  /*!
  * @return the height of the video
  */
  int getHeight() { return 20; };

};

/// Tells us to register our functionality to an engine kernel
extern "C" void registerPlugin(VIOKernel &K);


#endif
