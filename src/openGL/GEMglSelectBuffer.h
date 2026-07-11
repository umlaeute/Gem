/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLSELECTBUFFER_H_
#define _INCLUDE__GEM_OPENGL_GEMGLSELECTBUFFER_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglSelectBuffer
 KEYWORDS
        openGL  1
 DESCRIPTION
        wrapper for the openGL-function
        "glSelectBuffer( GLsizei size, GLuint *buffer)"
 */

class GEM_EXTERN GEMglSelectBuffer : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglSelectBuffer, GemGLBase);

public:
  // Constructor
  GEMglSelectBuffer (t_float);  // CON

protected:
  // Destructor
  virtual ~GEMglSelectBuffer ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);
  virtual void  postrender (GemState *state);

  // variables
  GLsizei       size;           // VAR
  virtual void  sizeMess(t_float);      // FUN

  int len;
  GLuint*       buffer;         // VAR

private:

  // we need some inlets
  t_inlet *m_inlet;
  t_outlet*m_bufout;

  // static member functions
  static void    sizeMessCallback (void*, t_float);
};
#endif // for header file
