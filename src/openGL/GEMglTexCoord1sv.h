/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLTEXCOORD1SV_H_
#define _INCLUDE__GEM_OPENGL_GEMGLTEXCOORD1SV_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglTexCoord1sv
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glTexCoord1sv( GLshort* v)"
 */

class GEM_EXTERN GEMglTexCoord1sv : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglTexCoord1sv, GemGLBase);

public:
  // Constructor
  GEMglTexCoord1sv (t_float);   // CON
protected:
  // Destructor
  virtual ~GEMglTexCoord1sv ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variable
  GLshort v[1];           // VAR
  virtual void    vMess(t_float); // FUN

private:

  // we need one inlet
  t_inlet *m_inlet;

  // static member functions
  static void    vMessCallback (void*, t_float);
};
#endif // for header file
