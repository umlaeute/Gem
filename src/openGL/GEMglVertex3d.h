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

#ifndef INCLUDE_GEM_GLVERTEX3D_H_
#define INCLUDE_GEM_GLVERTEX3D_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglVertex3d
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glVertex3d( GLdouble x, GLdouble y, GLdouble z)"
 */

class GEM_EXTERN GEMglVertex3d : public GemBase
{
	CPPEXTERN_HEADER(GEMglVertex3d, GemBase)

	public:
	  // Constructor
	  GEMglVertex3d (t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglVertex3d ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLdouble	x;		// VAR
	  virtual void	xMess(t_float);	// FUN

	  GLdouble	y;		// VAR
	  virtual void	yMess(t_float);	// FUN

	  GLdouble	z;		// VAR
	  virtual void	zMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[3];

	// static member functions
	  static void	 xMessCallback (void*, t_floatarg);
	  static void	 yMessCallback (void*, t_floatarg);
	  static void	 zMessCallback (void*, t_floatarg);
};
#endif // for header file
