/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLNORMAL3F_H_
#define _INCLUDE__GEM_OPENGL_GEMGLNORMAL3F_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglNormal3f
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glNormal3f( GLfloat nx, GLfloat ny, GLfloat nz)"
 */

class GEM_EXTERN GEMglNormal3f : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglNormal3f, GemGLBase);

public:
  // Constructor
  GEMglNormal3f (t_float, t_float, t_float);    // CON

protected:
  // Destructor
  virtual ~GEMglNormal3f ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLfloat       nx;             // VAR
  virtual void  nxMess(t_float);        // FUN

  GLfloat       ny;             // VAR
  virtual void  nyMess(t_float);        // FUN

  GLfloat       nz;             // VAR
  virtual void  nzMess(t_float);        // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[3];

  // static member functions
  static void    nxMessCallback (void*, t_float);
  static void    nyMessCallback (void*, t_float);
  static void    nzMessCallback (void*, t_float);
};
#endif // for header file
