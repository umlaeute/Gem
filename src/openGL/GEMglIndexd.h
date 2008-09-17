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

#ifndef INCLUDE_GEM_GLINDEXD_H_
#define INCLUDE_GEM_GLINDEXD_H_

#include "GemGLBase.h"

/*
 CLASS
	GEMglIndexd
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glIndexd( GLdouble c)"
 */

class GEM_EXTERN GEMglIndexd : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglIndexd, GemGLBase)

	public:
	  // Constructor
	  GEMglIndexd (t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglIndexd ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLdouble	c;		// VAR
	  virtual void	cMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[1];

	// static member functions
	  static void	 cMessCallback (void*, t_floatarg);
};
#endif // for header file
