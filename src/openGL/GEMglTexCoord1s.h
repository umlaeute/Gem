/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLTEXCOORD1S_H_
#define _INCLUDE__GEM_OPENGL_GEMGLTEXCOORD1S_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglTexCoord1s
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glTexCoord1s( GLshort s)"
 */

class GEM_EXTERN GEMglTexCoord1s : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglTexCoord1s, GemGLBase);

public:
  // Constructor
  GEMglTexCoord1s (t_float);    // CON

protected:
  // Destructor
  virtual ~GEMglTexCoord1s ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLshort       s;              // VAR
  virtual void  sMess(t_float); // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[1];

  // static member functions
  static void    sMessCallback (void*, t_float);
};
#endif // for header file
