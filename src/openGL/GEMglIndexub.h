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

#ifndef INCLUDE_GEM_GLINDEXUB_H_
#define INCLUDE_GEM_GLINDEXUB_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglIndexub
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glIndexub( GLubyte c)"
 */

class GEM_EXTERN GEMglIndexub : public GemBase
{
	CPPEXTERN_HEADER(GEMglIndexub, GemBase)

	public:
	  // Constructor
	  GEMglIndexub (t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglIndexub ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLubyte	c;		// VAR
	  virtual void	cMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[1];

	// static member functions
	  static void	 cMessCallback (void*, t_floatarg);
};
#endif // for header file
