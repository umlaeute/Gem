/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLINDEXS_H_
#define _INCLUDE__GEM_OPENGL_GEMGLINDEXS_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglIndexs
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glIndexs( GLshort c)"
 */

class GEM_EXTERN GEMglIndexs : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglIndexs, GemGLBase);

public:
  // Constructor
  GEMglIndexs (t_float);        // CON

protected:
  // Destructor
  virtual ~GEMglIndexs ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLshort       c;              // VAR
  virtual void  cMess(t_float); // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[1];

  // static member functions
  static void    cMessCallback (void*, t_float);
};
#endif // for header file
