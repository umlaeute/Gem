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
  : ErrorString(error)
{}

GemException::GemException() throw() 
  : ErrorString(std::string("")) 
{}
GemException::~GemException() throw() 
{}
const char *GemException::what() const throw() {
  return ErrorString.c_str();
}

void GemException::report(const char*origin) const throw() {
  if(!(ErrorString.empty())) {
    if (NULL==origin)
      error("GemException: %s", ErrorString.c_str());
    else
      error("[%s]: %s", origin, ErrorString.c_str());
  }
}


void gem::catchGemException() {
  try {
    throw;
  } catch (GemException &ex) {
    ex.report();
  }
}
