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

#ifndef INCLUDE_GEM_GLPOPATTRIB_H_
#define INCLUDE_GEM_GLPOPATTRIB_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglPopAttrib
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glPopAttrib()"
 */

class GEM_EXTERN GEMglPopAttrib : public GemBase
{
	CPPEXTERN_HEADER(GEMglPopAttrib, GemBase)

	public:
	  // Constructor
	  GEMglPopAttrib ();	// CON

	protected:
	  // Destructor
	  virtual ~GEMglPopAttrib ();
	  // Do the rendering
	  virtual void	render (GemState *state);
};
#endif // for header file
