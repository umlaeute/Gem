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

#ifndef INCLUDE_GEM_GLCOLOR3UBV_H_
#define INCLUDE_GEM_GLCOLOR3UBV_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglColor3ubv
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glColor3ubv( GLubyte* v)"
 */

class GEM_EXTERN GEMglColor3ubv : public GemBase
{
	CPPEXTERN_HEADER(GEMglColor3ubv, GemBase)

	public:
	  // Constructor
	  GEMglColor3ubv (t_float, t_float, t_float);	// CON
	protected:
	  // Destructor
	  virtual ~GEMglColor3ubv ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variable
	GLubyte	v[3];		// VAR
	virtual void	vMess(t_float, t_float, t_float);	// FUN

	private:

	// we need one inlet
	  t_inlet *m_inlet;

	// static member functions
         static void    vMessCallback (void*, t_floatarg, t_floatarg, t_floatarg);
};
#endif // for header file
