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

#ifndef INCLUDE_GEM_GLEVALPOINT1_H_
#define INCLUDE_GEM_GLEVALPOINT1_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglEvalPoint1
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glEvalPoint1( GLint i)"
 */

class GEM_EXTERN GEMglEvalPoint1 : public GemBase
{
	CPPEXTERN_HEADER(GEMglEvalPoint1, GemBase)

	public:
	  // Constructor
	  GEMglEvalPoint1 (t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglEvalPoint1 ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLint	i;		// VAR
	  virtual void	iMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[1];

	// static member functions
	  static void	 iMessCallback (void*, t_floatarg);
};
#endif // for header file
