//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The base class of the DeviceRead plugins.
//
//   DeviceRead
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

#include "DeviceRead.h"

namespace VideoIO_
{
  DeviceRead::DeviceRead()
  {
  }
  
  DeviceRead::~DeviceRead()
  {
  }
  
  bool DeviceRead::setChannel(int channel)
  {
    post("videoIO: Sorry, setting the channel is not supported.");
    return false;
  }
  
  bool DeviceRead::setFrequency(float frequency)
  {
    post("videoIO: Sorry, setting the channel is not supported.");
    return false;
  }
  
  bool DeviceRead::setTVNorm(char *norm)
  {
    post("videoIO: Sorry, setting the TV norm is not supported.");
    return false;
  }
  
}
