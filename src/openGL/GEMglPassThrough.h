/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLPASSTHROUGH_H_
#define _INCLUDE__GEM_OPENGL_GEMGLPASSTHROUGH_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglPassThrough
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glPassThrough( GLfloat token)"
 */

class GEM_EXTERN GEMglPassThrough : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglPassThrough, GemGLBase);

public:
  // Constructor
  GEMglPassThrough (t_float);   // CON

protected:
  // Destructor
  virtual ~GEMglPassThrough ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLfloat       token;          // VAR
  virtual void  tokenMess(t_float);     // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[1];

  // static member functions
  static void    tokenMessCallback (void*, t_float);
};
#endif // for header file
