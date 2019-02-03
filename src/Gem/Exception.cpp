////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 2009-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
// for NULL
#include <new>

#include "Exception.h"

// for error()
#include "m_pd.h"
#include <string.h>
#include <stdlib.h>

GemException::GemException(const char *error)
  : runtime_error(error)
{}

GemException::GemException(const std::string&error)
  : runtime_error(error)
{}

GemException::GemException()
  : runtime_error("")
{}

void GemException::report(const char*origin) const
{
  const char*ErrorString = what();
  if(ErrorString && *ErrorString) {
    if (NULL==origin) {
      error("GemException: %s", ErrorString);
    } else {
      error("[%s]: %s", origin, ErrorString);
    }
  }
}


void gem::catchGemException(const char*name, const t_object*obj)
{
  try {
    throw;
  } catch (GemException&ex) {
    if(NULL==obj) {
      ex.report(name);
    } else {
      t_object*o=(t_object*)obj;
      const char*str=ex.what();
      if(NULL!=str) {
        if (NULL==name) {
          pd_error(o, "GemException: %s", str);
        } else {
          pd_error(o, "[%s]: %s", name, str);
        }
      }
    }
  }
}
