/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLLOADIDENTITY_H_
#define _INCLUDE__GEM_OPENGL_GEMGLLOADIDENTITY_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglLoadIdentity
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glLoadIdentity()"
 */

class GEM_EXTERN GEMglLoadIdentity : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglLoadIdentity, GemGLBase);

public:
  // Constructor
  GEMglLoadIdentity (); // CON

protected:
  // Destructor
  virtual ~GEMglLoadIdentity ();
  // Do the rendering
  virtual void  render (GemState *state);
};
#endif // for header file
