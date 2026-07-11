/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLEDGEFLAG_H_
#define _INCLUDE__GEM_OPENGL_GEMGLEDGEFLAG_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglEdgeFlag
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glEdgeFlag( GLboolean flag)"
 */

class GEM_EXTERN GEMglEdgeFlag : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglEdgeFlag, GemGLBase);

public:
  // Constructor
  GEMglEdgeFlag (t_float);      // CON

protected:
  // Destructor
  virtual ~GEMglEdgeFlag ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLboolean     flag;           // VAR
  virtual void  flagMess(t_float);      // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[1];

  // static member functions
  static void    flagMessCallback (void*, t_float);
};
#endif // for header file
