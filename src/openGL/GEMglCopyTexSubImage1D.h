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

#ifndef _INCLUDE__GEM_OPENGL_GEMGLCOPYTEXSUBIMAGE1D_H_
#define _INCLUDE__GEM_OPENGL_GEMGLCOPYTEXSUBIMAGE1D_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglCopyTexSubImage1D
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glCopyTexSubImage1D( GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)"
 */

class GEM_EXTERN GEMglCopyTexSubImage1D : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglCopyTexSubImage1D, GemGLBase);

public:
  // Constructor
  GEMglCopyTexSubImage1D (int, t_atom*);        // CON

protected:
  // Destructor
  virtual ~GEMglCopyTexSubImage1D ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLenum        target;         // VAR
  virtual void  targetMess(t_float);    // FUN

  GLint level;          // VAR
  virtual void  levelMess(t_float);     // FUN

  GLint xoffset;                // VAR
  virtual void  xoffsetMess(t_float);   // FUN

  GLint x;              // VAR
  virtual void  xMess(t_float); // FUN

  GLint y;              // VAR
  virtual void  yMess(t_float); // FUN

  GLsizei       width;          // VAR
  virtual void  widthMess(t_float);     // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[6];

  // static member functions
  static void    targetMessCallback (void*, t_float);
  static void    levelMessCallback (void*, t_float);
  static void    xoffsetMessCallback (void*, t_float);
  static void    xMessCallback (void*, t_float);
  static void    yMessCallback (void*, t_float);
  static void    widthMessCallback (void*, t_float);
};
#endif // for header file
