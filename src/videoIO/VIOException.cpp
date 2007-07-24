//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The exception base class
//
//   VIOException
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

#include "VIOException.h"

namespace VideoIO_
{
  VIOException::VIOException(std::string error_message)
  {
    error_message_ = error_message;
  }
  
  void VIOException::postError()
  {
    post("Error: %s", error_message_.c_str());
  }
}
