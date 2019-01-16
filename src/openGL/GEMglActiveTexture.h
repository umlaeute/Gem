/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 *  Copyright (c) 2004-2005 tigital@mac.com
 *  For information on usage and redistribution, and for a DISCLAIMER
 *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLACTIVETEXTUREARB_H_
#define _INCLUDE__GEM_OPENGL_GEMGLACTIVETEXTUREARB_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglActiveTexture
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glActiveTexture( GLenum target )"
 */

class GEM_EXTERN GEMglActiveTexture : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglActiveTexture, GemGLBase);

public:
  // Constructor
  GEMglActiveTexture (t_float);      // CON

protected:
  // Destructor
  virtual ~GEMglActiveTexture ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLenum        texUnit;                // VAR
  virtual void  texUnitMess(t_float);   // FUN

private:

  // we need some inlets
  t_inlet *m_inlet[1];

  // static member functions
  static void    texUnitMessCallback (void*, t_float);
};
#endif // for header file
