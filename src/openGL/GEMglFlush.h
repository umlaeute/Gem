/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLFLUSH_H_
#define _INCLUDE__GEM_OPENGL_GEMGLFLUSH_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglFlush
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glFlush()"
 */

class GEM_EXTERN GEMglFlush : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglFlush, GemGLBase);

public:
  // Constructor
  GEMglFlush ();        // CON

protected:
  // Destructor
  virtual ~GEMglFlush ();
  // Do the rendering
  virtual void  render (GemState *state);

};
#endif // for header file
