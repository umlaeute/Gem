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

#ifndef INCLUDE_GEM_GLPOPCLIENTATTRIB_H_
#define INCLUDE_GEM_GLPOPCLIENTATTRIB_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglPopClientAttrib
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glPopClientAttrib( void)"
 */

class GEM_EXTERN GEMglPopClientAttrib : public GemBase
{
	CPPEXTERN_HEADER(GEMglPopClientAttrib, GemBase)

	public:
	  // Constructor
	  GEMglPopClientAttrib (void);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglPopClientAttrib ();
	  // Do the rendering
	  virtual void	render (GemState *state);
};
#endif // for header file
