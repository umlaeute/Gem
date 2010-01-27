
/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    The base class for exceptions thrown by Gem

    Copyright (c) 2009 IOhannes m zmölnig, zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/


/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GemException
    
    an exception class...

DESCRIPTION
    
    this is a class, we can throw on creation, 
    to make sure that the pd-object can not be created


    
-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMEXCEPTION_H_
#define INCLUDE_GEMEXCEPTION_H_

#include "Base/GemExportDef.h"
#include <string>

class GEM_EXTERN GemException
{
 public:
  GemException(void) throw();
  GemException(const char*error) throw();
  GemException(const std::string error) throw();
  virtual ~GemException(void) throw();
  
  virtual const char *what(void) const throw();
  virtual void report(const char*origin=0) const throw();
 private:
  const char*ErrorString;
};



#endif /* INCLUDE_GEMEXCEPTION_H_ */
