/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLVERTEX3IV_H_
#define _INCLUDE__GEM_OPENGL_GEMGLVERTEX3IV_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglVertex3iv
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glVertex3iv( GLint* v)"
 */

class GEM_EXTERN GEMglVertex3iv : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglVertex3iv, GemGLBase);

public:
  // Constructor
  GEMglVertex3iv (t_float, t_float, t_float);   // CON
protected:
  // Destructor
  virtual ~GEMglVertex3iv ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variable
  GLint   v[3];           // VAR
  virtual void    vMess(t_float, t_float, t_float);       // FUN

private:

  // we need one inlet
  t_inlet *m_inlet;

  // static member functions
  static void    vMessCallback (void*, t_float, t_float, t_float);
};
#endif // for header file
