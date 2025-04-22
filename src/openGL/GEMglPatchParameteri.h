/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 *  Copyright (c) 2002-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
 *     zmoelnig@iem.at
 *  For information on usage and redistribution, and for a DISCLAIMER
 *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
 *
 *  this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLPATCHPARAMETERI_H_
#define _INCLUDE__GEM_OPENGL_GEMGLPATCHPARAMETERI_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglPatchParameteri
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glPatchParameteri(GLint param)"
 */

class GEM_EXTERN GEMglPatchParameteri : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglPatchParameteri, GemGLBase);

public:
  // Constructor
  GEMglPatchParameteri (t_float); // CON

protected:
  // Destructor
  virtual ~GEMglPatchParameteri ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLint param;          // VAR
  virtual void  paramMess(t_float);     // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[1];

  // static member functions
  static void    paramMessCallback (void*, t_float);
};
#endif // for header file
