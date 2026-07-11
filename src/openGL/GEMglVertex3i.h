/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLVERTEX3I_H_
#define _INCLUDE__GEM_OPENGL_GEMGLVERTEX3I_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglVertex3i
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glVertex3i( GLint x, GLint y, GLint z)"
 */

class GEM_EXTERN GEMglVertex3i : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglVertex3i, GemGLBase);

public:
  // Constructor
  GEMglVertex3i (t_float, t_float, t_float);    // CON

protected:
  // Destructor
  virtual ~GEMglVertex3i ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLint x;              // VAR
  virtual void  xMess(t_float); // FUN

  GLint y;              // VAR
  virtual void  yMess(t_float); // FUN

  GLint z;              // VAR
  virtual void  zMess(t_float); // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[3];

  // static member functions
  static void    xMessCallback (void*, t_float);
  static void    yMessCallback (void*, t_float);
  static void    zMessCallback (void*, t_float);
};
#endif // for header file
