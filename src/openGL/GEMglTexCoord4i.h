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

#ifndef _INCLUDE__GEM_OPENGL_GEMGLTEXCOORD4I_H_
#define _INCLUDE__GEM_OPENGL_GEMGLTEXCOORD4I_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglTexCoord4i
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glTexCoord4i( GLint s, GLint t, GLint r, GLint q)"
 */

class GEM_EXTERN GEMglTexCoord4i : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglTexCoord4i, GemGLBase);

public:
  // Constructor
  GEMglTexCoord4i (t_float, t_float, t_float, t_float); // CON

protected:
  // Destructor
  virtual ~GEMglTexCoord4i ();
  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLint s;              // VAR
  virtual void  sMess(t_float); // FUN

  GLint t;              // VAR
  virtual void  tMess(t_float); // FUN

  GLint r;              // VAR
  virtual void  rMess(t_float); // FUN

  GLint q;              // VAR
  virtual void  qMess(t_float); // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[4];

  // static member functions
  static void    sMessCallback (void*, t_float);
  static void    tMessCallback (void*, t_float);
  static void    rMessCallback (void*, t_float);
  static void    qMessCallback (void*, t_float);
};
#endif // for header file
