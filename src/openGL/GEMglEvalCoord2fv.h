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

#ifndef INCLUDE_GEM_GLEVALCOORD2FV_H_
#define INCLUDE_GEM_GLEVALCOORD2FV_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglEvalCoord2fv
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glEvalCoord2fv( GLfloat *u glEvalCoord2fv GLfloat *u)"
 */

class GEM_EXTERN GEMglEvalCoord2fv : public GemBase
{
	CPPEXTERN_HEADER(GEMglEvalCoord2fv, GemBase)

	public:
	  // Constructor
	  GEMglEvalCoord2fv (t_float, t_float);	// CON
	protected:
	  // Destructor
	  virtual ~GEMglEvalCoord2fv ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variable
	GLfloat 	v[2];		// VAR
	virtual void	vMess(t_float, t_float);	// FUN

	private:

	// we need one inlet
	  t_inlet *m_inlet;

	// static member functions
         static void    vMessCallback (void*, t_floatarg, t_floatarg);
};
#endif // for header file
