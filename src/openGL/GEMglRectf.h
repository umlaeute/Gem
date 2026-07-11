/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLRECTF_H_
#define _INCLUDE__GEM_OPENGL_GEMGLRECTF_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglRectf
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glRectf( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)"
 */

class GEM_EXTERN GEMglRectf : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglRectf, GemGLBase);

public:
  // Constructor
  GEMglRectf (t_float, t_float, t_float, t_float);      // CON

protected:
  // Destructor
  virtual ~GEMglRectf ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLfloat       x1;             // VAR
  virtual void  x1Mess(t_float);        // FUN

  GLfloat       y1;             // VAR
  virtual void  y1Mess(t_float);        // FUN

  GLfloat       x2;             // VAR
  virtual void  x2Mess(t_float);        // FUN

  GLfloat       y2;             // VAR
  virtual void  y2Mess(t_float);        // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[4];

  // static member functions
  static void    x1MessCallback (void*, t_float);
  static void    y1MessCallback (void*, t_float);
  static void    x2MessCallback (void*, t_float);
  static void    y2MessCallback (void*, t_float);
};
#endif // for header file
