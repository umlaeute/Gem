/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLEVALCOORD2F_H_
#define _INCLUDE__GEM_OPENGL_GEMGLEVALCOORD2F_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglEvalCoord2f
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glEvalCoord2f( GLfloat u, GLfloat v)"
 */

class GEM_EXTERN GEMglEvalCoord2f : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglEvalCoord2f, GemGLBase);

public:
  // Constructor
  GEMglEvalCoord2f (t_float, t_float);  // CON

protected:
  // Destructor
  virtual ~GEMglEvalCoord2f ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLfloat       u;              // VAR
  virtual void  uMess(t_float); // FUN

  GLfloat       v;              // VAR
  virtual void  vMess(t_float); // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[2];

  // static member functions
  static void    uMessCallback (void*, t_float);
  static void    vMessCallback (void*, t_float);
};
#endif // for header file
