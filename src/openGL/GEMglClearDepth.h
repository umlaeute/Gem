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

#ifndef INCLUDE_GEM_GLCLEARDEPTH_H_
#define INCLUDE_GEM_GLCLEARDEPTH_H_

#include "GemGLBase.h"

/*
 CLASS
	GEMglClearDepth
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glClearDepth( GLclampd depth)"
 */

class GEM_EXTERN GEMglClearDepth : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglClearDepth, GemGLBase)

	public:
	  // Constructor
	  GEMglClearDepth (t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglClearDepth ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLclampd	depth;		// VAR
	  virtual void	depthMess(float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[1];

	// static member functions
	  static void	 depthMessCallback (void*, float);
};
#endif // for header file
