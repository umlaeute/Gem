 /* ------------------------------------------------------------------
  * GEM - Graphics Environment for Multimedia
  *
  *  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM
  *	zmoelnig@iem.kug.ac.at
  *  For information on usage and redistribution, and for a DISCLAIMER
  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
  *
  *  this file has been generated...
  * ------------------------------------------------------------------
  */

#ifndef INCLUDE_GEM_GLORTHO_H_
#define INCLUDE_GEM_GLORTHO_H_

#include "GemGLBase.h"

/*
 CLASS
	GEMglOrtho
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glOrtho( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)"
 */

class GEM_EXTERN GEMglOrtho : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglOrtho, GemGLBase);

	public:
	  // Constructor
	  GEMglOrtho (int, t_atom*);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglOrtho ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLdouble	left;		// VAR
	  virtual void	leftMess(t_float);	// FUN

	  GLdouble	right;		// VAR
	  virtual void	rightMess(t_float);	// FUN

	  GLdouble	bottom;		// VAR
	  virtual void	bottomMess(t_float);	// FUN

	  GLdouble	top;		// VAR
	  virtual void	topMess(t_float);	// FUN

	  GLdouble	zNear;		// VAR
	  virtual void	zNearMess(t_float);	// FUN

	  GLdouble	zFar;		// VAR
	  virtual void	zFarMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[6];

	// static member functions
	  static void	 leftMessCallback (void*, t_floatarg);
	  static void	 rightMessCallback (void*, t_floatarg);
	  static void	 bottomMessCallback (void*, t_floatarg);
	  static void	 topMessCallback (void*, t_floatarg);
	  static void	 zNearMessCallback (void*, t_floatarg);
	  static void	 zFarMessCallback (void*, t_floatarg);
};
#endif // for header file
