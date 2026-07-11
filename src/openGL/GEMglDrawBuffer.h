/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLDRAWBUFFER_H_
#define _INCLUDE__GEM_OPENGL_GEMGLDRAWBUFFER_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglDrawBuffer
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glDrawBuffer( GLenum mode)"
 */

class GEM_EXTERN GEMglDrawBuffer : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglDrawBuffer, GemGLBase);

public:
  // Constructor
  GEMglDrawBuffer (int, t_atom*); // CON

protected:
  // Destructor
  virtual ~GEMglDrawBuffer ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLenum        mode;           // VAR
  virtual void  modeMess(t_atom);       // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[1];

  // static member functions
  static void    modeMessCallback (void*,t_symbol*,int,t_atom*);
};
#endif // for header file
