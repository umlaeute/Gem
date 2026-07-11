/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLEVALCOORD2D_H_
#define _INCLUDE__GEM_OPENGL_GEMGLEVALCOORD2D_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglEvalCoord2d
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glEvalCoord2d( GLdouble u, GLdouble v)"
 */

class GEM_EXTERN GEMglEvalCoord2d : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglEvalCoord2d, GemGLBase);

public:
  // Constructor
  GEMglEvalCoord2d (t_float, t_float);  // CON

protected:
  // Destructor
  virtual ~GEMglEvalCoord2d ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLdouble      u;              // VAR
  virtual void  uMess(t_float); // FUN

  GLdouble      v;              // VAR
  virtual void  vMess(t_float); // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[2];

  // static member functions
  static void    uMessCallback (void*, t_float);
  static void    vMessCallback (void*, t_float);
};
#endif // for header file
