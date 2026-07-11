/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLFINISH_H_
#define _INCLUDE__GEM_OPENGL_GEMGLFINISH_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglFinish
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glFinish()"
 */

class GEM_EXTERN GEMglFinish : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglFinish, GemGLBase);

public:
  // Constructor
  GEMglFinish ();       // CON

protected:
  // Destructor
  virtual ~GEMglFinish ();
  // Do the rendering
  virtual void  render (GemState *state);

private:
};
#endif // for header file
