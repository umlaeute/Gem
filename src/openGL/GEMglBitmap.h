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

#ifndef INCLUDE_GEM_GLBITMAP_H_
#define INCLUDE_GEM_GLBITMAP_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglBitmap
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glBitmap( GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, GLubyte* bitmap)"
 */

class GEM_EXTERN GEMglBitmap : public GemBase
{
	CPPEXTERN_HEADER(GEMglBitmap, GemBase)

	public:
	  // Constructor
	  GEMglBitmap (t_float, t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglBitmap ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLfloat	xorig;		// VAR
	  virtual void	xorigMess(t_float);	// FUN

	  GLfloat	yorig;		// VAR
	  virtual void	yorigMess(t_float);	// FUN

	  GLfloat	xmove;		// VAR
	  virtual void	xmoveMess(t_float);	// FUN

	  GLfloat	ymove;		// VAR
	  virtual void	ymoveMess(t_float);	// FUN

	private:

	// we need some inlets
	  t_inlet *m_inlet[4];

	// static member functions
	  static void	 xorigMessCallback (void*, t_floatarg);
	  static void	 yorigMessCallback (void*, t_floatarg);
	  static void	 xmoveMessCallback (void*, t_floatarg);
	  static void	 ymoveMessCallback (void*, t_floatarg);
};
#endif // for header file
