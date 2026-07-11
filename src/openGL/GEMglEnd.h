/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLEND_H_
#define _INCLUDE__GEM_OPENGL_GEMGLEND_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglEnd
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glEnd()"
 */

class GEM_EXTERN GEMglEnd : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglEnd, GemGLBase);

public:
  // Constructor
  GEMglEnd ();  // CON

protected:
  // Destructor
  virtual ~GEMglEnd ();
  // Do the rendering
  virtual void  render (GemState *state);
};
#endif // for header file
