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

#ifndef INCLUDE_GEM_GLEND_H_
#define INCLUDE_GEM_GLEND_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglEnd
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glEnd()"
 */

class GEM_EXTERN GEMglEnd : public GemBase
{
	CPPEXTERN_HEADER(GEMglEnd, GemBase)

	public:
	  // Constructor
	  GEMglEnd ();	// CON

	protected:
	  // Destructor
	  virtual ~GEMglEnd ();
	  // Do the rendering
	  virtual void	render (GemState *state);
};
#endif // for header file
