/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLALPHAFUNC_H_
#define _INCLUDE__GEM_OPENGL_GEMGLALPHAFUNC_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglAlphaFunc
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glAlphaFunc( GLenum func, GLclampf ref)"
 */

class GEM_EXTERN GEMglAlphaFunc : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglAlphaFunc, GemGLBase);

public:
  // Constructor
  GEMglAlphaFunc (t_float, t_float);    // CON

protected:
  // Destructor
  virtual ~GEMglAlphaFunc ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLenum        func;           // VAR
  virtual void  funcMess(t_float);      // FUN

  GLclampf      ref;            // VAR
  virtual void  refMess(t_float);       // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[2];

  // static member functions
  static void    funcMessCallback (void*, t_float);
  static void    refMessCallback (void*, t_float);
};
#endif // for header file
