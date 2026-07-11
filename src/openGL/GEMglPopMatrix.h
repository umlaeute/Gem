/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLPOPMATRIX_H_
#define _INCLUDE__GEM_OPENGL_GEMGLPOPMATRIX_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglPopMatrix
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glPopMatrix()"
 */

class GEM_EXTERN GEMglPopMatrix : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglPopMatrix, GemGLBase);

public:
  // Constructor
  GEMglPopMatrix ();    // CON

protected:
  // Destructor
  virtual ~GEMglPopMatrix ();
  // Do the rendering
  virtual void  render (GemState *state);
};
#endif // for header file
