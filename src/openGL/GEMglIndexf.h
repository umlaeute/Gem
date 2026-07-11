/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLINDEXF_H_
#define _INCLUDE__GEM_OPENGL_GEMGLINDEXF_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglIndexf
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glIndexf( GLfloat c)"
 */

class GEM_EXTERN GEMglIndexf : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglIndexf, GemGLBase);

public:
  // Constructor
  GEMglIndexf (t_float);        // CON

protected:
  // Destructor
  virtual ~GEMglIndexf ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLfloat       c;              // VAR
  virtual void  cMess(t_float); // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[1];

  // static member functions
  static void    cMessCallback (void*, t_float);
};
#endif // for header file
