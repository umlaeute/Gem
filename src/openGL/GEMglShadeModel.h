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

#ifndef INCLUDE_GEM_GLSHADEMODEL_H_
#define INCLUDE_GEM_GLSHADEMODEL_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglShadeModel
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glShadeModel( GLenum mode)"
 */

class GEM_EXTERN GEMglShadeModel : public GemBase
{
	CPPEXTERN_HEADER(GEMglShadeModel, GemBase)

	public:
	  // Constructor
	  GEMglShadeModel (t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglShadeModel ();
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
