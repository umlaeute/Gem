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

#ifndef INCLUDE_GEM_GLINDEXUBV_H_
#define INCLUDE_GEM_GLINDEXUBV_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglIndexiv
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glIndexiv( GLint *c )"
 */

class GEM_EXTERN GEMglIndexiv : public GemBase
{
	CPPEXTERN_HEADER(GEMglIndexiv, GemBase)

	public:
	  // Constructor
	  GEMglIndexiv (t_floatarg);	// CON
	protected:
	  // Destructor
	  virtual ~GEMglIndexiv ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variable
	GLint c[1];		// VAR
	virtual void	cMess(t_float);	// FUN

	private:

	// we need one inlet
	  t_inlet *m_inlet;

	// static member functions
         static void    cMessCallback (void*, t_floatarg);
};
#endif // for header file
