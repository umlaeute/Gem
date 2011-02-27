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

#ifndef INCLUDE_GEM_GLTEXCOORD1D_H_
#define INCLUDE_GEM_GLTEXCOORD1D_H_

#include "GemGLBase.h"

/*
 CLASS
	GEMglTexCoord1d
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glTexCoord1d( GLdouble s)"
 */

class GEM_EXTERN GEMglTexCoord1d : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglTexCoord1d, GemGLBase);

	public:
	  // Constructor
	  GEMglTexCoord1d (t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglTexCoord1d ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLdouble	s;		// VAR
	  virtual void	sMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[1];

	// static member functions
	  static void	 sMessCallback (void*, t_floatarg);
};
#endif // for header file
