/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    a rendering context

    Copyright (c) 2009-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/
#ifndef _INCLUDE__GEM_BASE_GEMCONTEXT_H_
#define _INCLUDE__GEM_BASE_GEMCONTEXT_H_

#include "Gem/ExportDef.h"
#include "Gem/GemGL.h"

namespace gem
{
class GEM_EXTERN Context
{
private:
  class PIMPL;
  PIMPL*m_pimpl;

public:
  Context(void);
  Context(const Context&);
  virtual ~Context(void);

  Context&operator=(const Context&);

  // make context current
  bool push(void);

  // make context uncurrent
  bool pop(void);

public:
  static unsigned int getContextId(void);
  static void*getCurrentContext(void);
  static void*getCurrentXContext(void);

  static bool initializeXContext(void*display, int screen=0);
};

}; // namespace
#endif  // for header file
