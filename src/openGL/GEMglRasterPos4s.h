/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLRASTERPOS4S_H_
#define _INCLUDE__GEM_OPENGL_GEMGLRASTERPOS4S_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglRasterPos4s
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glRasterPos4s( GLshort x, GLshort y, GLshort z, GLshort w)"
 */

class GEM_EXTERN GEMglRasterPos4s : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglRasterPos4s, GemGLBase);

public:
  // Constructor
  GEMglRasterPos4s (t_float, t_float, t_float, t_float);        // CON

protected:
  // Destructor
  virtual ~GEMglRasterPos4s ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLshort       x;              // VAR
  virtual void  xMess(t_float); // FUN

  GLshort       y;              // VAR
  virtual void  yMess(t_float); // FUN

  GLshort       z;              // VAR
  virtual void  zMess(t_float); // FUN

  GLshort       w;              // VAR
  virtual void  wMess(t_float); // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[4];

  // static member functions
  static void    xMessCallback (void*, t_float);
  static void    yMessCallback (void*, t_float);
  static void    zMessCallback (void*, t_float);
  static void    wMessCallback (void*, t_float);
};
#endif // for header file
