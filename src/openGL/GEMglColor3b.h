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

#ifndef INCLUDE_GEM_GLCOLOR3B_H_
#define INCLUDE_GEM_GLCOLOR3B_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglColor3b
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glColor3b( GLbyte red, GLbyte green, GLbyte blue)"
 */

class GEM_EXTERN GEMglColor3b : public GemBase
{
	CPPEXTERN_HEADER(GEMglColor3b, GemBase)

	public:
	  // Constructor
	  GEMglColor3b (t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglColor3b ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLbyte	red;		// VAR
	  virtual void	redMess(t_float);	// FUN

	  GLbyte	green;		// VAR
	  virtual void	greenMess(t_float);	// FUN

	  GLbyte	blue;		// VAR
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
