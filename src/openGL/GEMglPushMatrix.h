/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLPUSHMATRIX_H_
#define _INCLUDE__GEM_OPENGL_GEMGLPUSHMATRIX_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglPushMatrix
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glPushMatrix()"
 */

class GEM_EXTERN GEMglPushMatrix : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglPushMatrix, GemGLBase);

public:
  // Constructor
  GEMglPushMatrix ();   // CON

protected:
  // Destructor
  virtual ~GEMglPushMatrix ();
  // Do the rendering
  virtual void  render (GemState *state);
};
#endif // for header file
