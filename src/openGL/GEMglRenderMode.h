/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLRENDERMODE_H_
#define _INCLUDE__GEM_OPENGL_GEMGLRENDERMODE_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglRenderMode
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glRenderMode( GLenum mode )"
 */

class GEM_EXTERN GEMglRenderMode : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglRenderMode, GemGLBase);

public:
  // Constructor
  GEMglRenderMode (int, t_atom*); // CON

protected:
  // Destructor
  virtual ~GEMglRenderMode ();
  // Do the rendering
  virtual void    render (GemState *state);

  // variables
  GLenum mode;            // VAR
  virtual void    modeMess(t_atom);       // FUN

  // we need some inlets
  t_inlet *m_inlet;
  t_outlet *m_outlet;

private:

  // static member functions
  static void      modeMessCallback (void*,t_symbol*,int,t_atom*);
};
#endif // for header file
