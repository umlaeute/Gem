/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLINDEXD_H_
#define _INCLUDE__GEM_OPENGL_GEMGLINDEXD_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglIndexd
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glIndexd( GLdouble c)"
 */

class GEM_EXTERN GEMglIndexd : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglIndexd, GemGLBase);

public:
  // Constructor
  GEMglIndexd (t_float);        // CON

protected:
  // Destructor
  virtual ~GEMglIndexd ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLdouble      c;              // VAR
  virtual void  cMess(t_float); // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[1];

  // static member functions
  static void    cMessCallback (void*, t_float);
};
#endif // for header file
