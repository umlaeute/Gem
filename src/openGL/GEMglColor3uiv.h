/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLCOLOR3UIV_H_
#define _INCLUDE__GEM_OPENGL_GEMGLCOLOR3UIV_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglColor3uiv
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glColor3uiv( GLuint* v)"
 */

class GEM_EXTERN GEMglColor3uiv : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglColor3uiv, GemGLBase);

public:
  // Constructor
  GEMglColor3uiv (t_float, t_float, t_float);   // CON
protected:
  // Destructor
  virtual ~GEMglColor3uiv ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variable
  GLuint  v[3];           // VAR
  virtual void    vMess(t_float, t_float, t_float);       // FUN

private:

  // we need one inlet
  t_inlet *m_inlet;

  // static member functions
  static void    vMessCallback (void*, t_float, t_float, t_float);
};
#endif // for header file
