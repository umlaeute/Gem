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

#ifndef INCLUDE_GEM_GLFINISH_H_
#define INCLUDE_GEM_GLFINISH_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglFinish
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glFinish()"
 */

class GEM_EXTERN GEMglFinish : public GemBase
{
	CPPEXTERN_HEADER(GEMglFinish, GemBase)

	public:
	  // Constructor
	  GEMglFinish ();	// CON

	protected:
	  // Destructor
	  virtual ~GEMglFinish ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	private:
};
#endif // for header file
