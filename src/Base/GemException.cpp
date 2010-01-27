////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2009 IOhannes m zmölnig.
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
// for NULL
#include <new>

#include "GemException.h"

// for error()
#include "m_pd.h"


GemException::GemException(const char *error) throw()
  : ErrorString(error)
{}

GemException::GemException(const std::string error) throw()
  : ErrorString(error.c_str())
{}

GemException::GemException() throw() 
  : ErrorString(NULL) 
{}
GemException::~GemException() throw() 
{}
const char *GemException::what() const throw() {
  return ErrorString;
}

void GemException::report(const char*origin) const throw() {
  if(ErrorString!=NULL) {
    if (NULL==origin)
      error("GemException: %s", ErrorString);
    else
      error("[%s]: %s", origin, ErrorString);
  }
}

