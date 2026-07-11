/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLCLIPPLANE_H_
#define _INCLUDE__GEM_OPENGL_GEMGLCLIPPLANE_H_

#include "Base/GemGLBase.h"

/*
  CLASS
  GEMglClipPlane
  KEYWORDS
  openGL        0
  DESCRIPTION
  wrapper for the openGL-function
  "glClipPlane( GLfloat* v)"
*/

class GEM_EXTERN GEMglClipPlane : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglClipPlane, GemGLBase);

public:
  // Constructor
  GEMglClipPlane (t_float, t_float, t_float, t_float);  // CON
protected:
  // Destructor
  virtual ~GEMglClipPlane ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variable
  GLdouble      v[4];           // VAR
  GLenum plane;
  virtual void  vMess(t_float, t_float, t_float, t_float);      // FUN
  virtual void  planeMess(t_float);     // FUN

private:

  // we need one inlet
  t_inlet *m_inlet[2];

  // static member functions
  static void    vMessCallback (void*, t_float, t_float, t_float, t_float);
  static void    planeMessCallback (void*, t_float);
};
#endif // for header file
