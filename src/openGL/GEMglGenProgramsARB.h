/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2004, James Tittle II and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLGENPROGRAMSARB_H_
#define _INCLUDE__GEM_OPENGL_GEMGLGENPROGRAMSARB_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglGenProgramsARB
 KEYWORDS
        openGL  1
 DESCRIPTION
        wrapper for the openGL-function
        "glGenProgramsARB( GLsizei n, GLuint *programs)"
 */

class GEM_EXTERN GEMglGenProgramsARB : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglGenProgramsARB, GemGLBase);

public:
  // Constructor
  GEMglGenProgramsARB (int,t_atom*);    // CON

protected:
  // Destructor
  virtual ~GEMglGenProgramsARB ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLsizei       n;              // VAR
  GLuint*       programs;               // VAR
  virtual void  programsMess(int,t_atom*);      // FUN


private:

  // we need some inlets
  t_inlet *m_inlet;

  // static member functions
  static void    programsMessCallback (void*, t_symbol*, int, t_atom*);
};
#endif // for header file
