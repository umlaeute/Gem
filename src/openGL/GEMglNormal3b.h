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

#ifndef INCLUDE_GEM_GLNORMAL3B_H_
#define INCLUDE_GEM_GLNORMAL3B_H_

#include "GemGLBase.h"

/*
 CLASS
	GEMglNormal3b
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glNormal3b( GLbyte nx, GLbyte ny, GLbyte nz)"
 */

class GEM_EXTERN GEMglNormal3b : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglNormal3b, GemGLBase)

	public:
	  // Constructor
	  GEMglNormal3b (t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglNormal3b ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLbyte	nx;		// VAR
	  virtual void	nxMess(t_float);	// FUN

	  GLbyte	ny;		// VAR
	  virtual void	nyMess(t_float);	// FUN

	  GLbyte	nz;		// VAR
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
