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

#ifndef INCLUDE_GEM_GLTEXSUBIMAGE1D_H_
#define INCLUDE_GEM_GLTEXSUBIMAGE1D_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglTexSubImage1D
 KEYWORDS
	openGL	1
 DESCRIPTION
	wrapper for the openGL-function
	"glTexSubImage1D( GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, GLvoid *pixels)"
 */

class GEM_EXTERN GEMglTexSubImage1D : public GemBase
{
	CPPEXTERN_HEADER(GEMglTexSubImage1D, GemBase)

	public:
	  // Constructor
	  GEMglTexSubImage1D (t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglTexSubImage1D ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	target;		// VAR
	  virtual void	targetMess(t_float);	// FUN

	  GLint	level;		// VAR
	  virtual void	levelMess(t_float);	// FUN

	  GLint	xoffset;		// VAR
	  virtual void	xoffsetMess(t_float);	// FUN

	  GLsizei	width;		// VAR
	  virtual void	widthMess(t_float);	// FUN

	private:

	// we need some inlets
	  t_inlet *m_inlet[3];

	// static member functions
	  static void	 targetMessCallback (void*, t_floatarg);
	  static void	 levelMessCallback (void*, t_floatarg);
	  static void	 xoffsetMessCallback (void*, t_floatarg);
	  static void	 widthMessCallback (void*, t_floatarg);
};
#endif // for header file
