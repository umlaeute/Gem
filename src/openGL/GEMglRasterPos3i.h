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

#ifndef INCLUDE_GEM_GLRASTERPOS3I_H_
#define INCLUDE_GEM_GLRASTERPOS3I_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglRasterPos3i
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glRasterPos3i( GLint x, GLint y, GLint z)"
 */

class GEM_EXTERN GEMglRasterPos3i : public GemBase
{
	CPPEXTERN_HEADER(GEMglRasterPos3i, GemBase)

	public:
	  // Constructor
	  GEMglRasterPos3i (t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglRasterPos3i ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLint	x;		// VAR
	  virtual void	xMess(t_float);	// FUN

	  GLint	y;		// VAR
	  virtual void	yMess(t_float);	// FUN

	  GLint	z;		// VAR
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
