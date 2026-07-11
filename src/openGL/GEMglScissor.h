/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLSCISSOR_H_
#define _INCLUDE__GEM_OPENGL_GEMGLSCISSOR_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglScissor
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glScissor( GLint x, GLint y, GLsizei width, GLsizei height)"
 */

class GEM_EXTERN GEMglScissor : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglScissor, GemGLBase);

public:
  // Constructor
  GEMglScissor (t_float, t_float, t_float, t_float);    // CON

protected:
  // Destructor
  virtual ~GEMglScissor ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLint x;              // VAR
  virtual void  xMess(t_float); // FUN

  GLint y;              // VAR
  virtual void  yMess(t_float); // FUN

  GLsizei       width;          // VAR
  virtual void  widthMess(t_float);     // FUN

  GLsizei       height;         // VAR
  virtual void  heightMess(t_float);    // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[4];

  // static member functions
  static void    xMessCallback (void*, t_float);
  static void    yMessCallback (void*, t_float);
  static void    widthMessCallback (void*, t_float);
  static void    heightMessCallback (void*, t_float);
};
#endif // for header file
