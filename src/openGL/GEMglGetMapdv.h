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

#ifndef INCLUDE_GEM_GLGETMAPDV_H_
#define INCLUDE_GEM_GLGETMAPDV_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglGetMapdv
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glGetMapdv( GLenum target, GLenum query, GLdouble *v)"
 */

class GEM_EXTERN GEMglGetMapdv : public GemBase
{
	CPPEXTERN_HEADER(GEMglGetMapdv, GemBase)

	public:
	  // Constructor
	  GEMglGetMapdv (t_floatarg, t_floatarg);	// CON
	protected:
	  // Destructor
	  virtual ~GEMglGetMapdv ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variable
	GLenum target;
	virtual void	targetMess(t_float);	// FUN

	GLenum query;
	virtual void	queryMess(t_float);	// FUN

	GLdouble v[4];

	private:

	// we need one inlet
	t_inlet *m_inlet[2];

	// static member functions
         static void    targetMessCallback(void*, t_floatarg);
         static void    queryMessCallback (void*, t_floatarg);
};
#endif // for header file
