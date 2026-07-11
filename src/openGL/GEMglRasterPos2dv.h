/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLRASTERPOS2DV_H_
#define _INCLUDE__GEM_OPENGL_GEMGLRASTERPOS2DV_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglRasterPos2dv
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glRasterPos2dv( GLdouble* v)"
 */

class GEM_EXTERN GEMglRasterPos2dv : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglRasterPos2dv, GemGLBase);

public:
  // Constructor
  GEMglRasterPos2dv (t_float, t_float); // CON
protected:
  // Destructor
  virtual ~GEMglRasterPos2dv ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variable
  GLdouble        v[2];           // VAR
  virtual void    vMess(t_float, t_float);        // FUN

private:

  // we need one inlet
  t_inlet *m_inlet;

  // static member functions
  static void    vMessCallback (void*, t_float, t_float);
};
#endif // for header file
