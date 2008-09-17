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

#ifndef INCLUDE_GEM_GLCOLOR4UB_H_
#define INCLUDE_GEM_GLCOLOR4UB_H_

#include "GemGLBase.h"

/*
 CLASS
	GEMglColor4ub
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glColor4ub( GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)"
 */

class GEM_EXTERN GEMglColor4ub : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglColor4ub, GemGLBase)

	public:
	  // Constructor
	  GEMglColor4ub (t_float, t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglColor4ub ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLubyte	red;		// VAR
	  virtual void	redMess(t_float);	// FUN

	  GLubyte	green;		// VAR
	  virtual void	greenMess(t_float);	// FUN

	  GLubyte	blue;		// VAR
	  virtual void	blueMess(t_float);	// FUN

	  GLubyte	alpha;		// VAR
	  virtual void	alphaMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[4];

	// static member functions
	  static void	 redMessCallback (void*, t_floatarg);
	  static void	 greenMessCallback (void*, t_floatarg);
	  static void	 blueMessCallback (void*, t_floatarg);
	  static void	 alphaMessCallback (void*, t_floatarg);
};
#endif // for header file
