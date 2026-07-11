/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLCALLLIST_H_
#define _INCLUDE__GEM_OPENGL_GEMGLCALLLIST_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglCallList
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glCallList( GLuint list )"
 */

class GEM_EXTERN GEMglCallList : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglCallList, GemGLBase);

public:
  // Constructor
  GEMglCallList (t_floatarg);   // CON

protected:
  // Destructor
  virtual ~GEMglCallList ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLuint list;          // VAR
  virtual void  listMess(t_float);      // FUN

private:

  // we need some inlets
  t_inlet *m_inlet;

  // static member functions
  static void    listMessCallback (void*, t_float);
};
#endif // for header file
