/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLDRAWARRAYS_H_
#define _INCLUDE__GEM_OPENGL_GEMGLDRAWARRAYS_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglDrawArrays
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glDrawArrays( GLenum mode, GLint first, GLsizei count)"
 */

class GEM_EXTERN GEMglDrawArrays : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglDrawArrays, GemGLBase);

public:
  // Constructor
  GEMglDrawArrays (t_float, t_float, t_float);  // CON

protected:
  // Destructor
  virtual ~GEMglDrawArrays ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLenum        mode;           // VAR
  virtual void  modeMess(t_float);      // FUN

  GLint first;          // VAR
  virtual void  firstMess(t_float);     // FUN

  GLsizei       count;          // VAR
  virtual void  countMess(t_float);     // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[3];

  // static member functions
  static void    modeMessCallback (void*, t_float);
  static void    firstMessCallback (void*, t_float);
  static void    countMessCallback (void*, t_float);
};
#endif // for header file
