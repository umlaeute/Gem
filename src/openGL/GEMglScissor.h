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

#ifndef INCLUDE_GEM_GLSCISSOR_H_
#define INCLUDE_GEM_GLSCISSOR_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglScissor
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glScissor( GLint x, GLint y, GLsizei width, GLsizei height)"
 */

class GEM_EXTERN GEMglScissor : public GemBase
{
	CPPEXTERN_HEADER(GEMglScissor, GemBase)

	public:
	  // Constructor
	  GEMglScissor (t_float, t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglScissor ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLint	x;		// VAR
	  virtual void	xMess(t_float);	// FUN

	  GLint	y;		// VAR
	  virtual void	yMess(t_float);	// FUN

	  GLsizei	width;		// VAR
	  virtual void	widthMess(t_float);	// FUN

	  GLsizei	height;		// VAR
	  virtual void	heightMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[4];

	// static member functions
	  static void	 xMessCallback (void*, t_floatarg);
	  static void	 yMessCallback (void*, t_floatarg);
	  static void	 widthMessCallback (void*, t_floatarg);
	  static void	 heightMessCallback (void*, t_floatarg);
};
#endif // for header file
