/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLLIGHTI_H_
#define _INCLUDE__GEM_OPENGL_GEMGLLIGHTI_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglLighti
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glLighti( GLenum light, GLenum pname, GLint param)"
 */

class GEM_EXTERN GEMglLighti : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglLighti, GemGLBase);

public:
  // Constructor
  GEMglLighti (t_float, t_float, t_float);      // CON

protected:
  // Destructor
  virtual ~GEMglLighti ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLenum        light;          // VAR
  virtual void  lightMess(t_float);     // FUN

  GLenum        pname;          // VAR
  virtual void  pnameMess(t_float);     // FUN

  GLint param;          // VAR
  virtual void  paramMess(t_float);     // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[3];

  // static member functions
  static void    lightMessCallback (void*, t_float);
  static void    pnameMessCallback (void*, t_float);
  static void    paramMessCallback (void*, t_float);
};
#endif // for header file
