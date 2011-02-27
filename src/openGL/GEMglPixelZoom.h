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

#ifndef INCLUDE_GEM_GLPIXELZOOM_H_
#define INCLUDE_GEM_GLPIXELZOOM_H_

#include "GemGLBase.h"

/*
 CLASS
	GEMglPixelZoom
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glPixelZoom( GLfloat xfactor, GLfloat yfactor)"
 */

class GEM_EXTERN GEMglPixelZoom : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglPixelZoom, GemGLBase);

	public:
	  // Constructor
	  GEMglPixelZoom (t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglPixelZoom ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLfloat	xfactor;		// VAR
	  virtual void	xfactorMess(t_float);	// FUN

	  GLfloat	yfactor;		// VAR
	  virtual void	yfactorMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[2];

	// static member functions
	  static void	 xfactorMessCallback (void*, t_floatarg);
	  static void	 yfactorMessCallback (void*, t_floatarg);
};
#endif // for header file
