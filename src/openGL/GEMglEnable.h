/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLENABLE_H_
#define _INCLUDE__GEM_OPENGL_GEMGLENABLE_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglEnable
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glEnable( GLenum cap)"
 */

class GEM_EXTERN GEMglEnable : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglEnable, GemGLBase);

public:
  // Constructor
  GEMglEnable (int, t_atom*); // CON

protected:
  // Destructor
  virtual ~GEMglEnable ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLenum        cap;            // VAR
  virtual void  capMess(t_atom);        // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[1];

  // static member functions
  static void    capMessCallback (void*,t_symbol*,int,t_atom*);
};
#endif // for header file
