/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLPUSHCLIENTATTRIB_H_
#define _INCLUDE__GEM_OPENGL_GEMGLPUSHCLIENTATTRIB_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglPushClientAttrib
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glPushClientAttrib( GLbitfield mask)"
 */

class GEM_EXTERN GEMglPushClientAttrib : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglPushClientAttrib, GemGLBase);

public:
  // Constructor
  GEMglPushClientAttrib (t_float);      // CON

protected:
  // Destructor
  virtual ~GEMglPushClientAttrib ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLbitfield    mask;           // VAR
  virtual void  maskMess(t_float);      // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[1];

  // static member functions
  static void    maskMessCallback (void*, t_float);
};
#endif // for header file
