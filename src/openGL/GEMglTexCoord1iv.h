/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLTEXCOORD1IV_H_
#define _INCLUDE__GEM_OPENGL_GEMGLTEXCOORD1IV_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglTexCoord1iv
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glTexCoord1iv( GLint* v)"
 */

class GEM_EXTERN GEMglTexCoord1iv : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglTexCoord1iv, GemGLBase);

public:
  // Constructor
  GEMglTexCoord1iv (t_float);   // CON
protected:
  // Destructor
  virtual ~GEMglTexCoord1iv ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variable
  GLint   v[1];           // VAR
  virtual void    vMess(t_float); // FUN

private:

  // we need one inlet
  t_inlet *m_inlet;

  // static member functions
  static void    vMessCallback (void*, t_float);
};
#endif // for header file
