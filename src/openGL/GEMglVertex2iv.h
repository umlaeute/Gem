/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLVERTEX2IV_H_
#define _INCLUDE__GEM_OPENGL_GEMGLVERTEX2IV_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglVertex2iv
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glVertex2iv( GLint* v)"
 */

class GEM_EXTERN GEMglVertex2iv : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglVertex2iv, GemGLBase);

public:
  // Constructor
  GEMglVertex2iv (t_float, t_float);    // CON
protected:
  // Destructor
  virtual ~GEMglVertex2iv ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variable
  GLint   v[2];           // VAR
  virtual void    vMess(t_float, t_float);        // FUN

private:

  // we need one inlet
  t_inlet *m_inlet;

  // static member functions
  static void    vMessCallback (void*, t_float, t_float);
};
#endif // for header file
