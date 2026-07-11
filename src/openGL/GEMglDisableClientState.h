/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLDISABLECLIENTSTATE_H_
#define _INCLUDE__GEM_OPENGL_GEMGLDISABLECLIENTSTATE_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglDisableClientState
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glDisableClientState( GLenum array)"
 */

class GEM_EXTERN GEMglDisableClientState : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglDisableClientState, GemGLBase);

public:
  // Constructor
  GEMglDisableClientState (int, t_atom*); // CON

protected:
  // Destructor
  virtual ~GEMglDisableClientState ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLenum        array;          // VAR
  virtual void  arrayMess(t_atom);      // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[1];

  // static member functions
  static void    arrayMessCallback (void*,t_symbol*,int,t_atom*);
};
#endif // for header file
