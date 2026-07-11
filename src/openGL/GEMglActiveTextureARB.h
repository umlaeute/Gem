/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2004, James Tittle II and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLACTIVETEXTUREARB_H_
#define _INCLUDE__GEM_OPENGL_GEMGLACTIVETEXTUREARB_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglActiveTextureARB
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glActiveTextureARB( GLenum target )"
 */

class GEM_EXTERN GEMglActiveTextureARB : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglActiveTextureARB, GemGLBase);

public:
  // Constructor
  GEMglActiveTextureARB (t_float);      // CON

protected:
  // Destructor
  virtual ~GEMglActiveTextureARB ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLenum        texUnit;                // VAR
  virtual void  texUnitMess(t_float);   // FUN

private:

  // we need some inlets
  t_inlet *m_inlet[1];

  // static member functions
  static void    texUnitMessCallback (void*, t_float);
};
#endif // for header file
