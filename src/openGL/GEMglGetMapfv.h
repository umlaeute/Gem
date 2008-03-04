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

#ifndef INCLUDE_GEM_GLGETMAPFV_H_
#define INCLUDE_GEM_GLGETMAPFV_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglGetMapfv
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glGetMapfv( GLenum target, GLenum query, GLfloat *v)"
 */

class GEM_EXTERN GEMglGetMapfv : public GemBase
{
	CPPEXTERN_HEADER(GEMglGetMapfv, GemBase)

	public:
	  // Constructor
	  GEMglGetMapfv (t_floatarg, t_floatarg);	// CON
	protected:
	  // Destructor
	  virtual ~GEMglGetMapfv ();
          // check extensions
          virtual bool isRunnable(void);

	  // Do the rendering
	  virtual void	render (GemState *state);

	// variable
	GLenum target;
	virtual void	targetMess(t_float);	// FUN

	GLenum query;
	virtual void	queryMess(t_float);	// FUN

	GLfloat v[4];

	private:

	// we need one inlet
	t_inlet *m_inlet[2];

	// static member functions
         static void    targetMessCallback(void*, t_floatarg);
         static void    queryMessCallback (void*, t_floatarg);
};
#endif // for header file
