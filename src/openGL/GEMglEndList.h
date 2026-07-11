/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLENDLIST_H_
#define _INCLUDE__GEM_OPENGL_GEMGLENDLIST_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglEndList
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glEndList()"
 */

class GEM_EXTERN GEMglEndList : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglEndList, GemGLBase);

public:
  // Constructor
  GEMglEndList ();      // CON

protected:
  // Destructor
  virtual ~GEMglEndList ();
  // Do the rendering
  virtual void  render (GemState *state);
};
#endif // for header file
