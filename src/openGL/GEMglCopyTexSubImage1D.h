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

#ifndef INCLUDE_GEM_GLCOPYTEXSUBIMAGE1D_H_
#define INCLUDE_GEM_GLCOPYTEXSUBIMAGE1D_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglCopyTexSubImage1D
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glCopyTexSubImage1D( GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)"
 */

class GEM_EXTERN GEMglCopyTexSubImage1D : public GemBase
{
	CPPEXTERN_HEADER(GEMglCopyTexSubImage1D, GemBase)

	public:
	  // Constructor
	  GEMglCopyTexSubImage1D (int, t_atom*);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglCopyTexSubImage1D ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	target;		// VAR
	  virtual void	targetMess(t_float);	// FUN

	  GLint	level;		// VAR
	  virtual void	levelMess(t_float);	// FUN

	  GLint	xoffset;		// VAR
	  virtual void	xoffsetMess(t_float);	// FUN

	  GLint	x;		// VAR
	  virtual void	xMess(t_float);	// FUN

	  GLint	y;		// VAR
	  virtual void	yMess(t_float);	// FUN

	  GLsizei	width;		// VAR
	  virtual void	widthMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[6];

	// static member functions
	  static void	 targetMessCallback (void*, t_floatarg);
	  static void	 levelMessCallback (void*, t_floatarg);
	  static void	 xoffsetMessCallback (void*, t_floatarg);
	  static void	 xMessCallback (void*, t_floatarg);
	  static void	 yMessCallback (void*, t_floatarg);
	  static void	 widthMessCallback (void*, t_floatarg);
};
#endif // for header file
