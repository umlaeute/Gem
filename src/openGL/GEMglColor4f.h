/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLCOLOR4F_H_
#define _INCLUDE__GEM_OPENGL_GEMGLCOLOR4F_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglColor4f
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glColor4f( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)"
 */

class GEM_EXTERN GEMglColor4f : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglColor4f, GemGLBase);

public:
  // Constructor
  GEMglColor4f (t_float, t_float, t_float, t_float);    // CON

protected:
  // Destructor
  virtual ~GEMglColor4f ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLfloat       red;            // VAR
  virtual void  redMess(t_float);       // FUN

  GLfloat       green;          // VAR
  virtual void  greenMess(t_float);     // FUN

  GLfloat       blue;           // VAR
  virtual void  blueMess(t_float);      // FUN

  GLfloat       alpha;          // VAR
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
