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

#ifndef INCLUDE_GEM_GLRECTS_H_
#define INCLUDE_GEM_GLRECTS_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglRects
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glRects( GLshort x1, GLshort y1, GLshort x2, GLshort y2)"
 */

class GEM_EXTERN GEMglRects : public GemBase
{
	CPPEXTERN_HEADER(GEMglRects, GemBase)

	public:
	  // Constructor
	  GEMglRects (t_float, t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglRects ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLshort	x1;		// VAR
	  virtual void	x1Mess(t_float);	// FUN

	  GLshort	y1;		// VAR
	  virtual void	y1Mess(t_float);	// FUN

	  GLshort	x2;		// VAR
	  virtual void	x2Mess(t_float);	// FUN

	  GLshort	y2;		// VAR
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
