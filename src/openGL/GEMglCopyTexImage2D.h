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

#ifndef INCLUDE_GEM_GLCOPYTEXIMAGE2D_H_
#define INCLUDE_GEM_GLCOPYTEXIMAGE2D_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglCopyTexImage2D
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glCopyTexImage2D( GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)"
 */

class GEM_EXTERN GEMglCopyTexImage2D : public GemBase
{
	CPPEXTERN_HEADER(GEMglCopyTexImage2D, GemBase)

	public:
	  // Constructor
	  GEMglCopyTexImage2D (int, t_atom*);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglCopyTexImage2D ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	target;		// VAR
	  virtual void	targetMess(t_float);	// FUN

	  GLint	level;		// VAR
	  virtual void	levelMess(t_float);	// FUN

	  GLenum	internalFormat;		// VAR
	  virtual void	internalFormatMess(t_float);	// FUN

	  GLint	x;		// VAR
	  virtual void	xMess(t_float);	// FUN

	  GLint	y;		// VAR
	  virtual void	yMess(t_float);	// FUN

	  GLsizei	width;		// VAR
	  virtual void	widthMess(t_float);	// FUN

	  GLsizei	height;		// VAR
	  virtual void	heightMess(t_float);	// FUN

	  GLint	border;		// VAR
	  virtual void	borderMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[8];

	// static member functions
	  static void	 targetMessCallback (void*, t_floatarg);
	  static void	 levelMessCallback (void*, t_floatarg);
	  static void	 internalFormatMessCallback (void*, t_floatarg);
	  static void	 xMessCallback (void*, t_floatarg);
	  static void	 yMessCallback (void*, t_floatarg);
	  static void	 widthMessCallback (void*, t_floatarg);
	  static void	 heightMessCallback (void*, t_floatarg);
	  static void	 borderMessCallback (void*, t_floatarg);
};
#endif // for header file
