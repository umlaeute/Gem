/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLMATERIALFV_H_
#define _INCLUDE__GEM_OPENGL_GEMGLMATERIALFV_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglMaterialfv
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glMaterialfv( GLenum face, GLenum pname, GLfloat*param)"
 */

class GEM_EXTERN GEMglMaterialfv : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglMaterialfv, GemGLBase);

public:
  // Constructor
  GEMglMaterialfv (int, t_atom*);       // CON

protected:
  // Destructor
  virtual ~GEMglMaterialfv ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLenum        face;           // VAR
  virtual void  faceMess(t_atom);       // FUN

  GLenum        pname;          // VAR
  virtual void  pnameMess(t_atom);      // FUN

  GLfloat       param[4];               // VAR
  virtual void  paramMess(int, t_atom*);        // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[3];

  // static member functions
  static void    faceMessCallback (void*, t_symbol*,int,t_atom*);
  static void    pnameMessCallback (void*, t_symbol*,int,t_atom*);
  static void    paramMessCallback (void*, t_symbol*,int,t_atom*);
};
#endif // for header file
