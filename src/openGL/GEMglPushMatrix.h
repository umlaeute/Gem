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

#ifndef INCLUDE_GEM_GLPUSHMATRIX_H_
#define INCLUDE_GEM_GLPUSHMATRIX_H_

#include "Base/GemGLBase.h"

/*
 CLASS
	GEMglPushMatrix
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glPushMatrix()"
 */

class GEM_EXTERN GEMglPushMatrix : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglPushMatrix, GemGLBase);

	public:
	  // Constructor
	  GEMglPushMatrix ();	// CON

	protected:
	  // Destructor
	  virtual ~GEMglPushMatrix ();
	  // Do the rendering
	  virtual void	render (GemState *state);
};
#endif // for header file
