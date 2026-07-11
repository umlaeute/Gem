/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLTEXCOORD2F_H_
#define _INCLUDE__GEM_OPENGL_GEMGLTEXCOORD2F_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglTexCoord2f
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glTexCoord2f( GLfloat s, GLfloat t)"
 */

class GEM_EXTERN GEMglTexCoord2f : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglTexCoord2f, GemGLBase);

public:
  // Constructor
  GEMglTexCoord2f (t_float, t_float);   // CON

protected:
  // Destructor
  virtual ~GEMglTexCoord2f ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLfloat       s;              // VAR
  virtual void  sMess(t_float); // FUN

  GLfloat       t;              // VAR
  virtual void  tMess(t_float); // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[2];

  // static member functions
  static void    sMessCallback (void*, t_float);
  static void    tMessCallback (void*, t_float);
};
#endif // for header file
