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

#ifndef INCLUDE_GEM_GLNORMAL3F_H_
#define INCLUDE_GEM_GLNORMAL3F_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglNormal3f
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glNormal3f( GLfloat nx, GLfloat ny, GLfloat nz)"
 */

class GEM_EXTERN GEMglNormal3f : public GemBase
{
	CPPEXTERN_HEADER(GEMglNormal3f, GemBase)

	public:
	  // Constructor
	  GEMglNormal3f (t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglNormal3f ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLfloat	nx;		// VAR
	  virtual void	nxMess(t_float);	// FUN

	  GLfloat	ny;		// VAR
	  virtual void	nyMess(t_float);	// FUN

	  GLfloat	nz;		// VAR
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
