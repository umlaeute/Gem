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

#ifndef INCLUDE_GEM_GLPASSTHROUGH_H_
#define INCLUDE_GEM_GLPASSTHROUGH_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglPassThrough
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glPassThrough( GLfloat token)"
 */

class GEM_EXTERN GEMglPassThrough : public GemBase
{
	CPPEXTERN_HEADER(GEMglPassThrough, GemBase)

	public:
	  // Constructor
	  GEMglPassThrough (t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglPassThrough ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLfloat	token;		// VAR
	  virtual void	tokenMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[1];

	// static member functions
	  static void	 tokenMessCallback (void*, t_floatarg);
};
#endif // for header file
