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

#ifndef INCLUDE_GEM_GLEND_H_
#define INCLUDE_GEM_GLEND_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglEndList
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glEndList()"
 */

class GEM_EXTERN GEMglEndList : public GemBase
{
	CPPEXTERN_HEADER(GEMglEndList, GemBase)

	public:
	  // Constructor
	  GEMglEndList ();	// CON

	protected:
	  // Destructor
	  virtual ~GEMglEndList ();
	  // Do the rendering
	  virtual void	render (GemState *state);
};
#endif // for header file
