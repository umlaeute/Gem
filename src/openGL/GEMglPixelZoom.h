/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLPIXELZOOM_H_
#define _INCLUDE__GEM_OPENGL_GEMGLPIXELZOOM_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglPixelZoom
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glPixelZoom( GLfloat xfactor, GLfloat yfactor)"
 */

class GEM_EXTERN GEMglPixelZoom : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglPixelZoom, GemGLBase);

public:
  // Constructor
  GEMglPixelZoom (t_float, t_float);    // CON

protected:
  // Destructor
  virtual ~GEMglPixelZoom ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLfloat       xfactor;                // VAR
  virtual void  xfactorMess(t_float);   // FUN

  GLfloat       yfactor;                // VAR
  virtual void  yfactorMess(t_float);   // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[2];

  // static member functions
  static void    xfactorMessCallback (void*, t_float);
  static void    yfactorMessCallback (void*, t_float);
};
#endif // for header file
