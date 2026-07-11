/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLCOLOR4USV_H_
#define _INCLUDE__GEM_OPENGL_GEMGLCOLOR4USV_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglColor4usv
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glColor4usv( GLushort* v)"
 */

class GEM_EXTERN GEMglColor4usv : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglColor4usv, GemGLBase);

public:
  // Constructor
  GEMglColor4usv (t_float, t_float, t_float, t_float);  // CON
protected:
  // Destructor
  virtual ~GEMglColor4usv ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variable
  GLushort        v[4];           // VAR
  virtual void    vMess(t_float, t_float, t_float, t_float);      // FUN

private:

  // we need one inlet
  t_inlet *m_inlet;

  // static member functions
  static void    vMessCallback (void*, t_float, t_float, t_float, t_float);
};
#endif // for header file
