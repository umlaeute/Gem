/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 *  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM
 *	zmoelnig@iem.kug.ac.at
 *  For information on usage and redistribution, and for a DISCLAIMER
 *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
 *
 *  this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef INCLUDE_GEM_GLClipPlane_H_
#define INCLUDE_GEM_GLClipPlane_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
  CLASS
  GEMglClipPlane
  KEYWORDS
  openGL	0
  DESCRIPTION
  wrapper for the openGL-function
  "glClipPlane( GLfloat* v)"
*/

class GEM_EXTERN GEMglClipPlane : public GemBase
{
  CPPEXTERN_HEADER(GEMglClipPlane, GemBase)

    public:
  // Constructor
  GEMglClipPlane (t_float, t_float, t_float, t_float);	// CON
 protected:
  // Destructor
  virtual ~GEMglClipPlane ();
  // Do the rendering
  virtual void	render (GemState *state);

  // variable
  GLdouble	v[4];		// VAR
  GLenum plane;
  virtual void	vMess(t_float, t_float, t_float, t_float);	// FUN
  virtual void	planeMess(t_float);	// FUN

 private:

  // we need one inlet
  t_inlet *m_inlet[2];

  // static member functions
  static void    vMessCallback (void*, t_floatarg, t_floatarg, t_floatarg, t_floatarg);
  static void    planeMessCallback (void*, t_floatarg);
};
#endif // for header file
