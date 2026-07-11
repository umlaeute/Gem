/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLFEEDBACKBUFFER_H_
#define _INCLUDE__GEM_OPENGL_GEMGLFEEDBACKBUFFER_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglFeedbackBuffer
 KEYWORDS
        openGL  1
 DESCRIPTION
        wrapper for the openGL-function
        "glFeedbackBuffer( GLsizei size, GLenum type, GLfloat *buffer)"
 */

class GEM_EXTERN GEMglFeedbackBuffer : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglFeedbackBuffer, GemGLBase);

public:
  // Constructor
  GEMglFeedbackBuffer (t_float, t_float);       // CON

protected:
  // Destructor
  virtual ~GEMglFeedbackBuffer ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLsizei       size;           // VAR
  virtual void  sizeMess(t_float);      // FUN

  GLenum        type;           // VAR
  virtual void  typeMess(t_float);      // FUN

  GLsizei       len;
  GLfloat      *buffer;
private:

  // we need some inlets
  t_inlet *m_inlet[2];

  // static member functions
  static void    sizeMessCallback (void*, t_float);
  static void    typeMessCallback (void*, t_float);
};
#endif // for header file
