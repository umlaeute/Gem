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

#ifndef INCLUDE_GEM_GLTEXCOORD3F_H_
#define INCLUDE_GEM_GLTEXCOORD3F_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglTexCoord3f
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glTexCoord3f( GLfloat s, GLfloat t, GLfloat r)"
 */

class GEM_EXTERN GEMglTexCoord3f : public GemBase
{
	CPPEXTERN_HEADER(GEMglTexCoord3f, GemBase)

	public:
	  // Constructor
	  GEMglTexCoord3f (t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglTexCoord3f ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLfloat	s;		// VAR
	  virtual void	sMess(t_float);	// FUN

	  GLfloat	t;		// VAR
	  virtual void	tMess(t_float);	// FUN

	  GLfloat	r;		// VAR
	  virtual void	rMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[3];

	// static member functions
	  static void	 sMessCallback (void*, t_floatarg);
	  static void	 tMessCallback (void*, t_floatarg);
	  static void	 rMessCallback (void*, t_floatarg);
};
#endif // for header file
