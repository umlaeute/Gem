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

#ifndef INCLUDE_GEM_GLPOPNAME_H_
#define INCLUDE_GEM_GLPOPNAME_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglPopName
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glPopName( void)"
 */

class GEM_EXTERN GEMglPopName : public GemBase
{
	CPPEXTERN_HEADER(GEMglPopName, GemBase)

	public:
	  // Constructor
	  GEMglPopName (void);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglPopName ();
	  // Do the rendering
	  virtual void	render (GemState *state);
};
#endif // for header file
