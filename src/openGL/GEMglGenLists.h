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

#ifndef INCLUDE_GEM_GLGENLISTS_H_
#define INCLUDE_GEM_GLGENLISTS_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglGenLists
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glGenLists( GLsizei range )"
 */

class GEM_EXTERN GEMglGenLists : public GemBase
{
	CPPEXTERN_HEADER(GEMglGenLists, GemBase)

	public:
	  // Constructor
	GEMglGenLists (t_floatarg);	// CON

	protected:
	  // Destructor
	virtual ~GEMglGenLists ();
	// Do the rendering
	virtual void	render (GemState *state);

	// variables
	GLsizei	range;		// VAR
	virtual void	rangeMess(t_float);	// FUN

	private:

	// we need some in/outlets
	t_inlet *m_inlet;
	t_outlet*m_outlet;

	// static member functions
	  static void	 rangeMessCallback (void*, t_floatarg);
};
#endif // for header file
