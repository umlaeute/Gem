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

#ifndef INCLUDE_GEM_GLDEPTHFUNC_H_
#define INCLUDE_GEM_GLDEPTHFUNC_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglDepthFunc
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glDepthFunc( GLenum func)"
 */

class GEM_EXTERN GEMglDepthFunc : public GemBase
{
	CPPEXTERN_HEADER(GEMglDepthFunc, GemBase)

	public:
	  // Constructor
	  GEMglDepthFunc (t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglDepthFunc ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	func;		// VAR
	  virtual void	funcMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[1];

	// static member functions
	  static void	 funcMessCallback (void*, t_floatarg);
};
#endif // for header file
