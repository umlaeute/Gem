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

#ifndef INCLUDE_GEM_GLNORMAL3S_H_
#define INCLUDE_GEM_GLNORMAL3S_H_

#include "GemGLBase.h"

/*
 CLASS
	GEMglNormal3s
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glNormal3s( GLshort nx, GLshort ny, GLshort nz)"
 */

class GEM_EXTERN GEMglNormal3s : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglNormal3s, GemGLBase)

	public:
	  // Constructor
	  GEMglNormal3s (t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglNormal3s ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLshort	nx;		// VAR
	  virtual void	nxMess(t_float);	// FUN

	  GLshort	ny;		// VAR
	  virtual void	nyMess(t_float);	// FUN

	  GLshort	nz;		// VAR
	  virtual void	nzMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[3];

	// static member functions
	  static void	 nxMessCallback (void*, t_floatarg);
	  static void	 nyMessCallback (void*, t_floatarg);
	  static void	 nzMessCallback (void*, t_floatarg);
};
#endif // for header file
