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

#ifndef INCLUDE_GEM_GLPOINTSIZE_H_
#define INCLUDE_GEM_GLPOINTSIZE_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglPointSize
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glPointSize( GLfloat size)"
 */

class GEM_EXTERN GEMglPointSize : public GemBase
{
	CPPEXTERN_HEADER(GEMglPointSize, GemBase)

	public:
	  // Constructor
	  GEMglPointSize (t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglPointSize ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLfloat	size;		// VAR
	  virtual void	sizeMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[1];

	// static member functions
	  static void	 sizeMessCallback (void*, t_floatarg);
};
#endif // for header file
