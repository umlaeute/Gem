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

#ifndef INCLUDE_GEM_GLPOPMATRIX_H_
#define INCLUDE_GEM_GLPOPMATRIX_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglPopMatrix
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glPopMatrix()"
 */

class GEM_EXTERN GEMglPopMatrix : public GemBase
{
	CPPEXTERN_HEADER(GEMglPopMatrix, GemBase)

	public:
	  // Constructor
	  GEMglPopMatrix ();	// CON

	protected:
	  // Destructor
	  virtual ~GEMglPopMatrix ();
	  // Do the rendering
	  virtual void	render (GemState *state);
};
#endif // for header file
