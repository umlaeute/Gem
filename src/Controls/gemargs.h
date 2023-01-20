/*-----------------------------------------------------------------
  LOG
  GEM - Graphics Environment for Multimedia

  ordered receive

  Copyright (c) 2008-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

  -----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_CONTROLS_GEMARGS_H_
#define _INCLUDE__GEM_CONTROLS_GEMARGS_H_

#include "Base/CPPExtern.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  gemargs

  split arguments passed to the containing abstraction on the ";"

  DESCRIPTION

  this object is really totally unrelated to Gem.
  however, it is included here, to be able to make Gem-abstractions behave
  the same as (or at least, similar to) Gem-builtins

  -----------------------------------------------------------------*/


class GEM_EXTERN gemargs : public CPPExtern
{
  CPPEXTERN_HEADER(gemargs, CPPExtern);

public:

  //////////
  // Constructor
  gemargs(int depth);

protected:

  //////////
  // Destructor
  virtual ~gemargs();

  //////////
  // forward all messages to the outlet
  void bang(void);

  const t_canvas*m_argcanvas;
  t_outlet*m_argout, *m_msgout;
};

#endif  // for header file
