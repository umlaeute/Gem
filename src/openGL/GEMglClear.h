/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLCLEAR_H_
#define _INCLUDE__GEM_OPENGL_GEMGLCLEAR_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglClear
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glClear( GLbitfield mask)"
 */

class GEM_EXTERN GEMglClear : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglClear, GemGLBase);

public:
  // Constructor
  GEMglClear (t_float); // CON

protected:
  // Destructor
  virtual ~GEMglClear ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLbitfield    mask;           // VAR
  virtual void  maskMess(t_float);      // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[1];

  // static member functions
  static void    maskMessCallback (void*, t_float);
};
#endif // for header file
