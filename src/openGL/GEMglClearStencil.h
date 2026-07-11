/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLCLEARSTENCIL_H_
#define _INCLUDE__GEM_OPENGL_GEMGLCLEARSTENCIL_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglClearStencil
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glClearStencil( GLint s)"
 */

class GEM_EXTERN GEMglClearStencil : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglClearStencil, GemGLBase);

public:
  // Constructor
  GEMglClearStencil (t_float);  // CON

protected:
  // Destructor
  virtual ~GEMglClearStencil ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLint s;              // VAR
  virtual void  sMess(t_float); // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[1];

  // static member functions
  static void    sMessCallback (void*, t_float);
};
#endif // for header file
