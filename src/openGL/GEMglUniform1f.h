/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLUNIFORM1FARB_H_
#define _INCLUDE__GEM_OPENGL_GEMGLUNIFORM1FARB_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglUniform1f
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glUniform1f( GLint location, GLfloat val)"
 */

class GEM_EXTERN GEMglUniform1f : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglUniform1f, GemGLBase);

public:
  // Constructor
  GEMglUniform1f (t_float, t_float); // CON

protected:
  // Destructor
  virtual ~GEMglUniform1f ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLint location;               // VAR
  virtual void  locMess(t_float);       // FUN

  GLfloat       val;            // VAR
  virtual void  valMess(t_float);       // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[2];

  // static member functions
  static void    locMessCallback (void*, t_float);
  static void    valMessCallback (void*, t_float);
};
#endif // for header file
