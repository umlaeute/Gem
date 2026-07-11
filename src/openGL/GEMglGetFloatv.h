/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLGETFLOATV_H_
#define _INCLUDE__GEM_OPENGL_GEMGLGETFLOATV_H_

#include "Base/GemGLBase.h"

/*
  CLASS
  GEMglGetFloatv
  KEYWORDS
  openGL        0
  DESCRIPTION
  wrapper for the openGL-function
  "glGetFloatv( GLenum pname, GLfloat *params)"
*/

class GEM_EXTERN GEMglGetFloatv : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglGetFloatv, GemGLBase);

public:
  // Constructor
  GEMglGetFloatv (int,t_atom*); // CON
protected:
  // Destructor
  virtual ~GEMglGetFloatv ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);
  // variable
  GLenum pname;
  virtual void  pnameMess(t_atom);      // FUN

  t_atom m_alist[16];

private:
  // we need one inlet
  t_inlet *m_inlet;
  // The outlet
  t_outlet      *m_outlet;

  // static member functions
  static void    pnameMessCallback (void*,t_symbol*,int,t_atom*);
};
#endif // for header file
