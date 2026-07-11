/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLEVALPOINT2_H_
#define _INCLUDE__GEM_OPENGL_GEMGLEVALPOINT2_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglEvalPoint2
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glEvalPoint2( GLint i, GLint j)"
 */

class GEM_EXTERN GEMglEvalPoint2 : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglEvalPoint2, GemGLBase);

public:
  // Constructor
  GEMglEvalPoint2 (t_float, t_float);   // CON

protected:
  // Destructor
  virtual ~GEMglEvalPoint2 ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLint i;              // VAR
  virtual void  iMess(t_float); // FUN

  GLint j;              // VAR
  virtual void  jMess(t_float); // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[2];

  // static member functions
  static void    iMessCallback (void*, t_float);
  static void    jMessCallback (void*, t_float);
};
#endif // for header file
