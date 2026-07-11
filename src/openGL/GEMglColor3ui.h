/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLCOLOR3UI_H_
#define _INCLUDE__GEM_OPENGL_GEMGLCOLOR3UI_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglColor3ui
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glColor3ui( GLuint red, GLuint green, GLuint blue)"
 */

class GEM_EXTERN GEMglColor3ui : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglColor3ui, GemGLBase);

public:
  // Constructor
  GEMglColor3ui (t_float, t_float, t_float);    // CON

protected:
  // Destructor
  virtual ~GEMglColor3ui ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLuint        red;            // VAR
  virtual void  redMess(t_float);       // FUN

  GLuint        green;          // VAR
  virtual void  greenMess(t_float);     // FUN

  GLuint        blue;           // VAR
  virtual void  blueMess(t_float);      // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[3];

  // static member functions
  static void    redMessCallback (void*, t_float);
  static void    greenMessCallback (void*, t_float);
  static void    blueMessCallback (void*, t_float);
};
#endif // for header file
