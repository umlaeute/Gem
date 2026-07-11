/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLRASTERPOS2I_H_
#define _INCLUDE__GEM_OPENGL_GEMGLRASTERPOS2I_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglRasterPos2i
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glRasterPos2i( GLint x, GLint y)"
 */

class GEM_EXTERN GEMglRasterPos2i : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglRasterPos2i, GemGLBase);

public:
  // Constructor
  GEMglRasterPos2i (t_float, t_float);  // CON

protected:
  // Destructor
  virtual ~GEMglRasterPos2i ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLint x;              // VAR
  virtual void  xMess(t_float); // FUN

  GLint y;              // VAR
  virtual void  yMess(t_float); // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[2];

  // static member functions
  static void    xMessCallback (void*, t_float);
  static void    yMessCallback (void*, t_float);
};
#endif // for header file
