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

#ifndef INCLUDE_GEM_GLVERTEX2F_H_
#define INCLUDE_GEM_GLVERTEX2F_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglVertex2f
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glVertex2f( GLfloat x, GLfloat y)"
 */

class GEM_EXTERN GEMglVertex2f : public GemBase
{
	CPPEXTERN_HEADER(GEMglVertex2f, GemBase)

	public:
	  // Constructor
	  GEMglVertex2f (t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglVertex2f ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLfloat	x;		// VAR
	  virtual void	xMess(t_float);	// FUN

	  GLfloat	y;		// VAR
	  virtual void	yMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[2];

	// static member functions
	  static void	 xMessCallback (void*, t_floatarg);
	  static void	 yMessCallback (void*, t_floatarg);
};
#endif // for header file
