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

#ifndef INCLUDE_GEM_GLRECTF_H_
#define INCLUDE_GEM_GLRECTF_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglRectf
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glRectf( GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)"
 */

class GEM_EXTERN GEMglRectf : public GemBase
{
	CPPEXTERN_HEADER(GEMglRectf, GemBase)

	public:
	  // Constructor
	  GEMglRectf (t_float, t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglRectf ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLfloat	x1;		// VAR
	  virtual void	x1Mess(t_float);	// FUN

	  GLfloat	y1;		// VAR
	  virtual void	y1Mess(t_float);	// FUN

	  GLfloat	x2;		// VAR
	  virtual void	x2Mess(t_float);	// FUN

	  GLfloat	y2;		// VAR
	  virtual void	y2Mess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[4];

	// static member functions
	  static void	 x1MessCallback (void*, t_floatarg);
	  static void	 y1MessCallback (void*, t_floatarg);
	  static void	 x2MessCallback (void*, t_floatarg);
	  static void	 y2MessCallback (void*, t_floatarg);
};
#endif // for header file
