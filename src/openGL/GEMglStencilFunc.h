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

#ifndef INCLUDE_GEM_GLSTENCILFUNC_H_
#define INCLUDE_GEM_GLSTENCILFUNC_H_

#include "GemGLBase.h"

/*
 CLASS
	GEMglStencilFunc
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glStencilFunc( GLenum func, GLint ref, GLuint mask)"
 */

class GEM_EXTERN GEMglStencilFunc : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglStencilFunc, GemGLBase)

	public:
	  // Constructor
	  GEMglStencilFunc (t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglStencilFunc ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	func;		// VAR
	  virtual void	funcMess(t_float);	// FUN

	  GLint	ref;		// VAR
	  virtual void	refMess(t_float);	// FUN

	  GLuint	mask;		// VAR
	  virtual void	maskMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[3];

	// static member functions
	  static void	 funcMessCallback (void*, t_floatarg);
	  static void	 refMessCallback (void*, t_floatarg);
	  static void	 maskMessCallback (void*, t_floatarg);
};
#endif // for header file
