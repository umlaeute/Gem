/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLPOPNAME_H_
#define _INCLUDE__GEM_OPENGL_GEMGLPOPNAME_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglPopName
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glPopName( void)"
 */

class GEM_EXTERN GEMglPopName : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglPopName, GemGLBase);

public:
  // Constructor
  GEMglPopName (void);  // CON

protected:
  // Destructor
  virtual ~GEMglPopName ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);
};
#endif // for header file
