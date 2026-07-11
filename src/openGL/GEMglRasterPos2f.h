/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLRASTERPOS2F_H_
#define _INCLUDE__GEM_OPENGL_GEMGLRASTERPOS2F_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglRasterPos2f
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glRasterPos2f( GLfloat x, GLfloat y)"
 */

class GEM_EXTERN GEMglRasterPos2f : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglRasterPos2f, GemGLBase);

public:
  // Constructor
  GEMglRasterPos2f (t_float, t_float);  // CON

protected:
  // Destructor
  virtual ~GEMglRasterPos2f ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLfloat       x;              // VAR
  virtual void  xMess(t_float); // FUN

  GLfloat       y;              // VAR
  virtual void  yMess(t_float); // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[2];

  // static member functions
  static void    xMessCallback (void*, t_float);
  static void    yMessCallback (void*, t_float);
};
#endif // for header file
