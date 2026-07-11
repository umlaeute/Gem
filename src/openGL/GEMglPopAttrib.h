/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLPOPATTRIB_H_
#define _INCLUDE__GEM_OPENGL_GEMGLPOPATTRIB_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglPopAttrib
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glPopAttrib()"
 */

class GEM_EXTERN GEMglPopAttrib : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglPopAttrib, GemGLBase);

public:
  // Constructor
  GEMglPopAttrib ();    // CON

protected:
  // Destructor
  virtual ~GEMglPopAttrib ();
  // Do the rendering
  virtual void  render (GemState *state);
};
#endif // for header file
