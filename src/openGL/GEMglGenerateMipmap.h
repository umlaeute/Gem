/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLGENERATEMIPMAP_H_
#define _INCLUDE__GEM_OPENGL_GEMGLGENERATEMIPMAP_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglGenerateMipmap
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glGenerateMipmap( GLenum target)"
 */

class GEM_EXTERN GEMglGenerateMipmap : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglGenerateMipmap, GemGLBase);

public:
  // Constructor
  GEMglGenerateMipmap (int, t_atom*); // CON

protected:
  // Destructor
  virtual ~GEMglGenerateMipmap ();
  // Do the rendering
  virtual bool isRunnable(void);
  virtual void  render (GemState *state);

  // variables
  GLenum        target;            // VAR
  virtual void  targetMess(t_atom&);        // FUN

private:

  // we need some inlets
  t_inlet *m_inlet[1];

  // static member functions
  static void    targetMessCallback (void*,t_symbol*,int,t_atom*);
};
#endif // for header file
