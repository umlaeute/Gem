/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLCOLOR4B_H_
#define _INCLUDE__GEM_OPENGL_GEMGLCOLOR4B_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglColor4b
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glColor4b( GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha)"
 */

class GEM_EXTERN GEMglColor4b : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglColor4b, GemGLBase);

public:
  // Constructor
  GEMglColor4b (t_float, t_float, t_float, t_float);    // CON

protected:
  // Destructor
  virtual ~GEMglColor4b ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLbyte        red;            // VAR
  virtual void  redMess(t_float);       // FUN

  GLbyte        green;          // VAR
  virtual void  greenMess(t_float);     // FUN

  GLbyte        blue;           // VAR
  virtual void  blueMess(t_float);      // FUN

  GLbyte        alpha;          // VAR
  virtual void  alphaMess(t_float);     // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[4];

  // static member functions
  static void    redMessCallback (void*, t_float);
  static void    greenMessCallback (void*, t_float);
  static void    blueMessCallback (void*, t_float);
  static void    alphaMessCallback (void*, t_float);
};
#endif // for header file
