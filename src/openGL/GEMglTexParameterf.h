/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLTEXPARAMETERF_H_
#define _INCLUDE__GEM_OPENGL_GEMGLTEXPARAMETERF_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglTexParameterf
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glTexParameterf( GLenum target, GLenum pname, GLfloat param)"
 */

class GEM_EXTERN GEMglTexParameterf : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglTexParameterf, GemGLBase);

public:
  // Constructor
  GEMglTexParameterf (t_float, t_float, t_float);       // CON

protected:
  // Destructor
  virtual ~GEMglTexParameterf ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLenum        target;         // VAR
  virtual void  targetMess(t_float);    // FUN

  GLenum        pname;          // VAR
  virtual void  pnameMess(t_float);     // FUN

  GLfloat       param;          // VAR
  virtual void  paramMess(t_float);     // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[3];

  // static member functions
  static void    targetMessCallback (void*, t_float);
  static void    pnameMessCallback (void*, t_float);
  static void    paramMessCallback (void*, t_float);
};
#endif // for header file
