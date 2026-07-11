/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLGENTEXTURES_H_
#define _INCLUDE__GEM_OPENGL_GEMGLGENTEXTURES_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglGenTextures
 KEYWORDS
        openGL  1
 DESCRIPTION
        wrapper for the openGL-function
        "glGenTextures( GLsizei n, GLuint *textures)"
 */

class GEM_EXTERN GEMglGenTextures : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglGenTextures, GemGLBase);

public:
  // Constructor
  GEMglGenTextures (int,t_atom*);       // CON

protected:
  // Destructor
  virtual ~GEMglGenTextures ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLsizei       n;              // VAR
  GLuint*       textures;               // VAR
  virtual void  texturesMess(int,t_atom*);      // FUN


private:

  // we need some inlets
  t_inlet *m_inlet;

  // static member functions
  static void    texturesMessCallback (void*, t_symbol*, int, t_atom*);
};
#endif // for header file
