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

#ifndef INCLUDE_GEM_GLALPHAFUNC_H_
#define INCLUDE_GEM_GLALPHAFUNC_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglAlphaFunc
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glAlphaFunc( GLenum func, GLclampf ref)"
 */

class GEM_EXTERN GEMglAlphaFunc : public GemBase
{
	CPPEXTERN_HEADER(GEMglAlphaFunc, GemBase)

	public:
	  // Constructor
	  GEMglAlphaFunc (t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglAlphaFunc ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	func;		// VAR
	  virtual void	funcMess(t_float);	// FUN

	  GLclampf	ref;		// VAR
	  virtual void	refMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[2];

	// static member functions
	  static void	 funcMessCallback (void*, t_floatarg);
	  static void	 refMessCallback (void*, t_floatarg);
};
#endif // for header file
