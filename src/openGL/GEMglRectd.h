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

#ifndef INCLUDE_GEM_GLRECTD_H_
#define INCLUDE_GEM_GLRECTD_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglRectd
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glRectd( GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)"
 */

class GEM_EXTERN GEMglRectd : public GemBase
{
	CPPEXTERN_HEADER(GEMglRectd, GemBase)

	public:
	  // Constructor
	  GEMglRectd (t_float, t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglRectd ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLdouble	x1;		// VAR
	  virtual void	x1Mess(t_float);	// FUN

	  GLdouble	y1;		// VAR
	  virtual void	y1Mess(t_float);	// FUN

	  GLdouble	x2;		// VAR
	  virtual void	x2Mess(t_float);	// FUN

	  GLdouble	y2;		// VAR
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
