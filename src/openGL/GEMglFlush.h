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

#ifndef INCLUDE_GEM_GLFLUSH_H_
#define INCLUDE_GEM_GLFLUSH_H_

#include "GemGLBase.h"

/*
 CLASS
	GEMglFlush
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glFlush()"
 */

class GEM_EXTERN GEMglFlush : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglFlush, GemGLBase);

	public:
	  // Constructor
	  GEMglFlush ();	// CON

	protected:
	  // Destructor
	  virtual ~GEMglFlush ();
	  // Do the rendering
	  virtual void	render (GemState *state);

};
#endif // for header file
