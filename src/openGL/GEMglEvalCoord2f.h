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

#ifndef INCLUDE_GEM_GLEVALCOORD2F_H_
#define INCLUDE_GEM_GLEVALCOORD2F_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglEvalCoord2f
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glEvalCoord2f( GLfloat u, GLfloat v)"
 */

class GEM_EXTERN GEMglEvalCoord2f : public GemBase
{
	CPPEXTERN_HEADER(GEMglEvalCoord2f, GemBase)

	public:
	  // Constructor
	  GEMglEvalCoord2f (t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglEvalCoord2f ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLfloat	u;		// VAR
	  virtual void	uMess(t_float);	// FUN

	  GLfloat	v;		// VAR
	  virtual void	vMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[2];

	// static member functions
	  static void	 uMessCallback (void*, t_floatarg);
	  static void	 vMessCallback (void*, t_floatarg);
};
#endif // for header file
