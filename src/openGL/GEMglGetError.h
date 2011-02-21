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

#ifndef INCLUDE_GEM_GLGETERROR_H_
#define INCLUDE_GEM_GLGETERROR_H_

#include "GemGLBase.h"

/*
 CLASS
	GEMglGetError
 KEYWORDS
	openGL	1
 DESCRIPTION
	wrapper for the openGL-function
	"glGetError( void )"
 */

class GEM_EXTERN GEMglGetError : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglGetError, GemGLBase)

	public:
	// Constructor
	GEMglGetError ();	// CON

 protected:
	// Destructor
	virtual ~GEMglGetError ();
	// Do the rendering
	virtual void	render (GemState *state);

	// we need some inlets
	t_outlet *m_outlet;
};
#endif // for header file
