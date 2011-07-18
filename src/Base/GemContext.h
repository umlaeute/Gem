/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    a rendering context

    Copyright (c) 2009 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/
#ifndef INCLUDE_GEMCONTEXT_H_
#define INCLUDE_GEMCONTEXT_H_

#include "Gem/ExportDef.h"
#include "Base/GemGL.h"


# ifdef __APPLE__
#  define GemGlewXContext void
# elif defined _WIN32
typedef struct WGLEWContextStruct WGLEWContext;
#  define GemGlewXContext WGLEWContext
# elif defined __linux__
typedef struct GLXEWContextStruct GLXEWContext;
#  define GemGlewXContext GLXEWContext
# endif

typedef struct GLEWContextStruct GLEWContext;

namespace gem {
class GEM_EXTERN Context {
 private:
  class PIMPL;
  PIMPL*m_pimpl;

 public:
  Context(void);
  Context(const Context&);
  virtual ~Context(void);

  // make context current
  bool push(void);

  // make context uncurrent
  bool pop(void);


 public:
  static unsigned int getContextId(void);
  static GLEWContext*getGlewContext(void);
  static GemGlewXContext*getGlewXContext(void);
};

}; // namespace
#endif  // for header file
