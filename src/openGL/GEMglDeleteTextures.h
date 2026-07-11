/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLDELETETEXTURES_H_
#define _INCLUDE__GEM_OPENGL_GEMGLDELETETEXTURES_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglDeleteTextures
 KEYWORDS
        openGL  1
 DESCRIPTION
        wrapper for the openGL-function
        "glDeleteTextures( GLsizei n, GLuint *textures)"
 */

class GEM_EXTERN GEMglDeleteTextures : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglDeleteTextures, GemGLBase);

public:
  // Constructor
  GEMglDeleteTextures (int,t_atom*);    // CON

protected:
  // Destructor
  virtual ~GEMglDeleteTextures ();
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
  static void    texturesMessCallback (void*, t_symbol*, int,t_atom*);
};
#endif // for header file
