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

#ifndef INCLUDE_GEM_GLSTENCILOP_H_
#define INCLUDE_GEM_GLSTENCILOP_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglStencilOp
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glStencilOp( GLenum fail, GLenum zfail, GLenum zpass)"
 */

class GEM_EXTERN GEMglStencilOp : public GemBase
{
	CPPEXTERN_HEADER(GEMglStencilOp, GemBase)

	public:
	  // Constructor
	  GEMglStencilOp (t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglStencilOp ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	fail;		// VAR
	  virtual void	failMess(t_float);	// FUN

	  GLenum	zfail;		// VAR
	  virtual void	zfailMess(t_float);	// FUN

	  GLenum	zpass;		// VAR
	  virtual void	zpassMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[3];

	// static member functions
	  static void	 failMessCallback (void*, t_floatarg);
	  static void	 zfailMessCallback (void*, t_floatarg);
	  static void	 zpassMessCallback (void*, t_floatarg);
};
#endif // for header file
