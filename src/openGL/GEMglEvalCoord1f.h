/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLEVALCOORD1F_H_
#define _INCLUDE__GEM_OPENGL_GEMGLEVALCOORD1F_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglEvalCoord1f
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glEvalCoord1f( GLfloat u)"
 */

class GEM_EXTERN GEMglEvalCoord1f : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglEvalCoord1f, GemGLBase);

public:
  // Constructor
  GEMglEvalCoord1f (t_float);   // CON

protected:
  // Destructor
  virtual ~GEMglEvalCoord1f ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLfloat       u;              // VAR
  virtual void  uMess(t_float); // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[1];

  // static member functions
  static void    uMessCallback (void*, t_float);
};
#endif // for header file
