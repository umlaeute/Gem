/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLGETERROR_H_
#define _INCLUDE__GEM_OPENGL_GEMGLGETERROR_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglGetError
 KEYWORDS
        openGL  1
 DESCRIPTION
        wrapper for the openGL-function
        "glGetError( void )"
 */

class GEM_EXTERN GEMglGetError : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglGetError, GemGLBase);

public:
  // Constructor
  GEMglGetError ();       // CON

protected:
  // Destructor
  virtual ~GEMglGetError ();
  // Do the rendering
  virtual void    render (GemState *state);

  // we need some inlets
  t_outlet *m_outlet;
};
#endif // for header file
