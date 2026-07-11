/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLMATERIALI_H_
#define _INCLUDE__GEM_OPENGL_GEMGLMATERIALI_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglMateriali
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glMateriali( GLenum face, GLenum pname, GLint param)"
 */

class GEM_EXTERN GEMglMateriali : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglMateriali, GemGLBase);

public:
  // Constructor
  GEMglMateriali (t_float, t_float, t_float);   // CON

protected:
  // Destructor
  virtual ~GEMglMateriali ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLenum        face;           // VAR
  virtual void  faceMess(t_float);      // FUN

  GLenum        pname;          // VAR
  virtual void  pnameMess(t_float);     // FUN

  GLint param;          // VAR
  virtual void  paramMess(t_float);     // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[3];

  // static member functions
  static void    faceMessCallback (void*, t_float);
  static void    pnameMessCallback (void*, t_float);
  static void    paramMessCallback (void*, t_float);
};
#endif // for header file
