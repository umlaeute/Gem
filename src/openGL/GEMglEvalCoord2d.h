 /* ------------------------------------------------------------------
  * GEM - Graphics Environment for Multimedia
  *
  *  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM
  *	zmoelnig@iem.kug.ac.at
  *  For information on usage and redistribution, and for a DISCLAIMER
  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
  *
  *  this file has been generated...
  * ------------------------------------------------------------------
  */

#ifndef INCLUDE_GEM_GLEVALCOORD2D_H_
#define INCLUDE_GEM_GLEVALCOORD2D_H_

#include "GemGLBase.h"

/*
 CLASS
	GEMglEvalCoord2d
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glEvalCoord2d( GLdouble u, GLdouble v)"
 */

class GEM_EXTERN GEMglEvalCoord2d : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglEvalCoord2d, GemGLBase)

	public:
	  // Constructor
	  GEMglEvalCoord2d (t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglEvalCoord2d ();
          // check extensions
          virtual bool isRunnable(void);

	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLdouble	u;		// VAR
	  virtual void	uMess(t_float);	// FUN

	  GLdouble	v;		// VAR
	  virtual void	vMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[2];

	// static member functions
	  static void	 uMessCallback (void*, t_floatarg);
	  static void	 vMessCallback (void*, t_floatarg);
};
#endif // for header file
