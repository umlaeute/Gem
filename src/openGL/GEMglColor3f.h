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

#ifndef INCLUDE_GEM_GLCOLOR3F_H_
#define INCLUDE_GEM_GLCOLOR3F_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglColor3f
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glColor3f( GLfloat red, GLfloat green, GLfloat blue)"
 */

class GEM_EXTERN GEMglColor3f : public GemBase
{
	CPPEXTERN_HEADER(GEMglColor3f, GemBase)

	public:
	  // Constructor
	  GEMglColor3f (t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglColor3f ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLfloat	red;		// VAR
	  virtual void	redMess(t_float);	// FUN

	  GLfloat	green;		// VAR
	  virtual void	greenMess(t_float);	// FUN

	  GLfloat	blue;		// VAR
	  virtual void	blueMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[3];

	// static member functions
	  static void	 redMessCallback (void*, t_floatarg);
	  static void	 greenMessCallback (void*, t_floatarg);
	  static void	 blueMessCallback (void*, t_floatarg);
};
#endif // for header file
