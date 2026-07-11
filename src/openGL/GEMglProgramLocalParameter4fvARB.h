/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2004, James Tittle II and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLPROGRAMLOCALPARAMETER4FVARB_H_
#define _INCLUDE__GEM_OPENGL_GEMGLPROGRAMLOCALPARAMETER4FVARB_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglProgramLocalParameter4fvARB
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glProgramLocalParameter4fvARB( GLenum target, GLuint index, GLfloat *params)"
 */

class GEM_EXTERN GEMglProgramLocalParameter4fvARB : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglProgramLocalParameter4fvARB, GemGLBase);

public:
  // Constructor
  GEMglProgramLocalParameter4fvARB (t_float, t_float, t_float); // CON

protected:
  // Destructor
  virtual ~GEMglProgramLocalParameter4fvARB ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLenum        target;         // VAR
  virtual void  targetMess(t_float);    // FUN

  GLuint        index;          // VAR
  virtual void  indexMess(t_float);     // FUN

  GLfloat       params[4];              // VAR
  virtual void  paramsMess(int argc, t_atom* argv);     // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[3];

  // static member functions
  static void    targetMessCallback (void*, t_float);
  static void    indexMessCallback (void*, t_float);
  static void    paramsMessCallback (void*, t_symbol*,int, t_atom*);
};
#endif // for header file
