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

#ifndef INCLUDE_GEM_GLBLENDFUNC_H_
#define INCLUDE_GEM_GLBLENDFUNC_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglBlendFunc
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glBlendFunc( GLenum sfactor, GLenum dfactor)"
 */

class GEM_EXTERN GEMglBlendFunc : public GemBase
{
	CPPEXTERN_HEADER(GEMglBlendFunc, GemBase)

	public:
	  // Constructor
	  GEMglBlendFunc (t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglBlendFunc ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	sfactor;		// VAR
	  virtual void	sfactorMess(t_float);	// FUN

	  GLenum	dfactor;		// VAR
	  virtual void	dfactorMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[2];

	// static member functions
	  static void	 sfactorMessCallback (void*, t_floatarg);
	  static void	 dfactorMessCallback (void*, t_floatarg);
};
#endif // for header file
