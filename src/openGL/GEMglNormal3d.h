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

#ifndef INCLUDE_GEM_GLNORMAL3D_H_
#define INCLUDE_GEM_GLNORMAL3D_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglNormal3d
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glNormal3d( GLdouble nx, GLdouble ny, GLdouble nz)"
 */

class GEM_EXTERN GEMglNormal3d : public GemBase
{
	CPPEXTERN_HEADER(GEMglNormal3d, GemBase)

	public:
	  // Constructor
	  GEMglNormal3d (t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglNormal3d ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLdouble	nx;		// VAR
	  virtual void	nxMess(t_float);	// FUN

	  GLdouble	ny;		// VAR
	  virtual void	nyMess(t_float);	// FUN

	  GLdouble	nz;		// VAR
	  virtual void	nzMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[3];

	// static member functions
	  static void	 nxMessCallback (void*, t_floatarg);
	  static void	 nyMessCallback (void*, t_floatarg);
	  static void	 nzMessCallback (void*, t_floatarg);
};
#endif // for header file
