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

#ifndef INCLUDE_GEM_GLVERTEX4I_H_
#define INCLUDE_GEM_GLVERTEX4I_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglVertex4i
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glVertex4i( GLint x, GLint y, GLint z, GLint w)"
 */

class GEM_EXTERN GEMglVertex4i : public GemBase
{
	CPPEXTERN_HEADER(GEMglVertex4i, GemBase)

	public:
	  // Constructor
	  GEMglVertex4i (t_float, t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglVertex4i ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLint	x;		// VAR
	  virtual void	xMess(t_float);	// FUN

	  GLint	y;		// VAR
	  virtual void	yMess(t_float);	// FUN

	  GLint	z;		// VAR
	  virtual void	zMess(t_float);	// FUN

	  GLint	w;		// VAR
	  virtual void	wMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[4];

	// static member functions
	  static void	 xMessCallback (void*, t_floatarg);
	  static void	 yMessCallback (void*, t_floatarg);
	  static void	 zMessCallback (void*, t_floatarg);
	  static void	 wMessCallback (void*, t_floatarg);
};
#endif // for header file
