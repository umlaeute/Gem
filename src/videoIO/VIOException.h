//////////////////////////////////////////////////////////////////////////
//
//   VideoIO-Framework for GEM/PD
//
//   The exception base class
//
//   VIOException
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

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <string>
#include "m_pd.h"

namespace VideoIO_
{
  class VIOException
  {
    public:
    /// constructor
    VIOException(std::string error_message);
    
    /// destroctor
    virtual ~VIOException(){};
    
    /// posts the error messages
    void postError();
    
    protected:
    
    /// here the error message is stored
    std::string error_message_;
    
    
    
  };
}

























#endif
