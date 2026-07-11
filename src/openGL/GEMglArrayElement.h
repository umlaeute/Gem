/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLARRAYELEMENT_H_
#define _INCLUDE__GEM_OPENGL_GEMGLARRAYELEMENT_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglArrayElement
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glArrayElement( GLint i)"
 */

class GEM_EXTERN GEMglArrayElement : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglArrayElement, GemGLBase);

public:
  // Constructor
  GEMglArrayElement (t_float);  // CON

protected:
  // Destructor
  virtual ~GEMglArrayElement ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLint i;              // VAR
  virtual void  iMess(t_float); // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[1];

  // static member functions
  static void    iMessCallback (void*, t_float);
};
#endif // for header file
