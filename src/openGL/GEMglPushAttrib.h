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

#ifndef INCLUDE_GEM_GLPUSHATTRIB_H_
#define INCLUDE_GEM_GLPUSHATTRIB_H_

#include "GemGLBase.h"

/*
 CLASS
	GEMglPushAttrib
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glPushAttrib( GLbitfield mask)"
 */

class GEM_EXTERN GEMglPushAttrib : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglPushAttrib, GemGLBase)

	public:
	  // Constructor
	  GEMglPushAttrib (t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglPushAttrib ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLbitfield	mask;		// VAR
	  virtual void	maskMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[1];

	// static member functions
	  static void	 maskMessCallback (void*, t_floatarg);
};
#endif // for header file
