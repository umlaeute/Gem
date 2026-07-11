/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLPIXELSTOREF_H_
#define _INCLUDE__GEM_OPENGL_GEMGLPIXELSTOREF_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglPixelStoref
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glPixelStoref( GLenum pname, GLfloat param)"
 */

class GEM_EXTERN GEMglPixelStoref : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglPixelStoref, GemGLBase);

public:
  // Constructor
  GEMglPixelStoref (t_float, t_float);  // CON

protected:
  // Destructor
  virtual ~GEMglPixelStoref ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLenum        pname;          // VAR
  virtual void  pnameMess(t_float);     // FUN

  GLfloat       param;          // VAR
  virtual void  paramMess(t_float);     // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[2];

  // static member functions
  static void    pnameMessCallback (void*, t_float);
  static void    paramMessCallback (void*, t_float);
};
#endif // for header file
