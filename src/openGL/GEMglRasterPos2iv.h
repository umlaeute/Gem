 /* ------------------------------------------------------------------
  * GEM - Graphics Environment for Multimedia
  *
  *  Copyright (c) 2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
  *	zmoelnig@iem.kug.ac.at
  *  For information on usage and redistribution, and for a DISCLAIMER
  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
  *
  *  this file has been generated...
  * ------------------------------------------------------------------
  */

#ifndef INCLUDE_GEM_GLRASTERPOS2IV_H_
#define INCLUDE_GEM_GLRASTERPOS2IV_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglRasterPos2iv
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glRasterPos2iv( GLint* v)"
 */

class GEM_EXTERN GEMglRasterPos2iv : public GemBase
{
	CPPEXTERN_HEADER(GEMglRasterPos2iv, GemBase)

	public:
	  // Constructor
	  GEMglRasterPos2iv (t_float, t_float);	// CON
	protected:
	  // Destructor
	  virtual ~GEMglRasterPos2iv ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variable
	GLint	v[2];		// VAR
	virtual void	vMess(t_float, t_float);	// FUN

	private:

	// we need one inlet
	  t_inlet *m_inlet;

	// static member functions
         static void    vMessCallback (void*, t_floatarg, t_floatarg);
};
#endif // for header file
