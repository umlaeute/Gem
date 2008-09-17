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

#ifndef INCLUDE_GEM_GLENABLE_H_
#define INCLUDE_GEM_GLENABLE_H_

#include "GemGLBase.h"

/*
 CLASS
	GEMglEnable
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glEnable( GLenum cap)"
 */

class GEM_EXTERN GEMglEnable : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglEnable, GemGLBase)

	public:
	  // Constructor
	  GEMglEnable (t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglEnable ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	cap;		// VAR
	  virtual void	capMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[1];

	// static member functions
	  static void	 capMessCallback (void*, t_floatarg);
};
#endif // for header file
