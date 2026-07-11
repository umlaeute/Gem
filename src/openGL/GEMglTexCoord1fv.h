/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLTEXCOORD1FV_H_
#define _INCLUDE__GEM_OPENGL_GEMGLTEXCOORD1FV_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglTexCoord1fv
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glTexCoord1fv( GLfloat* v)"
 */

class GEM_EXTERN GEMglTexCoord1fv : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglTexCoord1fv, GemGLBase);

public:
  // Constructor
  GEMglTexCoord1fv (t_float);   // CON
protected:
  // Destructor
  virtual ~GEMglTexCoord1fv ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variable
  GLfloat v[1];           // VAR
  virtual void    vMess(t_float); // FUN

private:

  // we need one inlet
  t_inlet *m_inlet;

  // static member functions
  static void    vMessCallback (void*, t_float);
};
#endif // for header file
