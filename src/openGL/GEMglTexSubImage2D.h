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

#ifndef INCLUDE_GEM_GLTEXSUBIMAGE2D_H_
#define INCLUDE_GEM_GLTEXSUBIMAGE2D_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglTexSubImage2D
 KEYWORDS
	openGL	1
 DESCRIPTION
	wrapper for the openGL-function
	"glTexSubImage2D( GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels)"
 */

class GEM_EXTERN GEMglTexSubImage2D : public GemBase
{
	CPPEXTERN_HEADER(GEMglTexSubImage2D, GemBase)

	public:
	  // Constructor
	  GEMglTexSubImage2D (int,t_atom*);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglTexSubImage2D ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	target;		// VAR
	  virtual void	targetMess(t_float);	// FUN

	  GLint	level;		// VAR
	  virtual void	levelMess(t_float);	// FUN

	  GLint	xoffset;		// VAR
	  virtual void	xoffsetMess(t_float);	// FUN

	  GLint	yoffset;		// VAR
	  virtual void	yoffsetMess(t_float);	// FUN

	  GLsizei	width;		// VAR
	  virtual void	widthMess(t_float);	// FUN

	  GLsizei	height;		// VAR
	  virtual void	heightMess(t_float);	// FUN

	private:

	// we need some inlets
	  t_inlet *m_inlet[5];

	// static member functions
	  static void	 targetMessCallback (void*, t_floatarg);
	  static void	 levelMessCallback (void*, t_floatarg);
	  static void	 xoffsetMessCallback (void*, t_floatarg);
	  static void	 yoffsetMessCallback (void*, t_floatarg);
	  static void	 widthMessCallback (void*, t_floatarg);
	  static void	 heightMessCallback (void*, t_floatarg);
};
#endif // for header file
