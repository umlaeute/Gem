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

#ifndef INCLUDE_GEM_GLFRONTFACE_H_
#define INCLUDE_GEM_GLFRONTFACE_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglFrontFace
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glFrontFace( GLenum mode)"
 */

class GEM_EXTERN GEMglFrontFace : public GemBase
{
	CPPEXTERN_HEADER(GEMglFrontFace, GemBase)

	public:
	  // Constructor
	  GEMglFrontFace (t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglFrontFace ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	mode;		// VAR
	  virtual void	modeMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[1];

	// static member functions
	  static void	 modeMessCallback (void*, t_floatarg);
};
#endif // for header file
