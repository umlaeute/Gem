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

#ifndef _INCLUDE__GEM_OPENGL_GEMGLGETMAPDV_H_
#define _INCLUDE__GEM_OPENGL_GEMGLGETMAPDV_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglGetMapdv
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glGetMapdv( GLenum target, GLenum query, GLdouble *v)"
 */

class GEM_EXTERN GEMglGetMapdv : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglGetMapdv, GemGLBase);

public:
  // Constructor
  GEMglGetMapdv (t_floatarg, t_floatarg);       // CON
protected:
  // Destructor
  virtual ~GEMglGetMapdv ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);

  // variable
  GLenum target;
  virtual void    targetMess(t_float);    // FUN

  GLenum query;
  virtual void    queryMess(t_float);     // FUN

  GLdouble v[4];

private:

  // we need one inlet
  t_inlet *m_inlet[2];

  // static member functions
  static void    targetMessCallback(void*, t_float);
  static void    queryMessCallback (void*, t_float);
};
#endif // for header file
