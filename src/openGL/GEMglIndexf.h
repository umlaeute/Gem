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

#ifndef INCLUDE_GEM_GLINDEXF_H_
#define INCLUDE_GEM_GLINDEXF_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglIndexf
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glIndexf( GLfloat c)"
 */

class GEM_EXTERN GEMglIndexf : public GemBase
{
	CPPEXTERN_HEADER(GEMglIndexf, GemBase)

	public:
	  // Constructor
	  GEMglIndexf (t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglIndexf ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLfloat	c;		// VAR
	  virtual void	cMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[1];

	// static member functions
	  static void	 cMessCallback (void*, t_floatarg);
};
#endif // for header file
