/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLEVALCOORD1FV_H_
#define _INCLUDE__GEM_OPENGL_GEMGLEVALCOORD1FV_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglEvalCoord1fv
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glEvalCoord1fv( GLfloat *u glEvalCoord1fv GLfloat *u)"
 */

class GEM_EXTERN GEMglEvalCoord1fv : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglEvalCoord1fv, GemGLBase);

public:
  // Constructor
  GEMglEvalCoord1fv (t_float);  // CON
protected:
  // Destructor
  virtual ~GEMglEvalCoord1fv ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);

  // variable
  GLfloat         v[1];           // VAR
  virtual void    vMess(t_float); // FUN

private:

  // we need one inlet
  t_inlet *m_inlet;

  // static member functions
  static void    vMessCallback (void*, t_float);
};
#endif // for header file
