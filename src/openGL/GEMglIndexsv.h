/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLINDEXSV_H_
#define _INCLUDE__GEM_OPENGL_GEMGLINDEXSV_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglIndexsv
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glIndexsv( GLshort *c )"
 */

class GEM_EXTERN GEMglIndexsv : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglIndexsv, GemGLBase);

public:
  // Constructor
  GEMglIndexsv (t_floatarg);    // CON
protected:
  // Destructor
  virtual ~GEMglIndexsv ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variable
  GLshort c[1];           // VAR
  virtual void    cMess(t_float); // FUN

private:

  // we need one inlet
  t_inlet *m_inlet;

  // static member functions
  static void    cMessCallback (void*, t_float);
};
#endif // for header file
