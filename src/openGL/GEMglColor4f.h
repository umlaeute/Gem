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

#ifndef INCLUDE_GEM_GLCOLOR4F_H_
#define INCLUDE_GEM_GLCOLOR4F_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglColor4f
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glColor4f( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)"
 */

class GEM_EXTERN GEMglColor4f : public GemBase
{
	CPPEXTERN_HEADER(GEMglColor4f, GemBase)

	public:
	  // Constructor
	  GEMglColor4f (t_float, t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglColor4f ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLfloat	red;		// VAR
	  virtual void	redMess(t_float);	// FUN

	  GLfloat	green;		// VAR
	  virtual void	greenMess(t_float);	// FUN

	  GLfloat	blue;		// VAR
	  virtual void	blueMess(t_float);	// FUN

	  GLfloat	alpha;		// VAR
	  virtual void	alphaMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[4];

	// static member functions
	  static void	 redMessCallback (void*, t_floatarg);
	  static void	 greenMessCallback (void*, t_floatarg);
	  static void	 blueMessCallback (void*, t_floatarg);
	  static void	 alphaMessCallback (void*, t_floatarg);
};
#endif // for header file
