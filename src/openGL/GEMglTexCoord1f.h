/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLTEXCOORD1F_H_
#define _INCLUDE__GEM_OPENGL_GEMGLTEXCOORD1F_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglTexCoord1f
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glTexCoord1f( GLfloat s)"
 */

class GEM_EXTERN GEMglTexCoord1f : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglTexCoord1f, GemGLBase);

public:
  // Constructor
  GEMglTexCoord1f (t_float);    // CON

protected:
  // Destructor
  virtual ~GEMglTexCoord1f ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLfloat       s;              // VAR
  virtual void  sMess(t_float); // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[1];

  // static member functions
  static void    sMessCallback (void*, t_float);
};
#endif // for header file
