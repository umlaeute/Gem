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

#ifndef INCLUDE_GEM_GLCLEARDEPTH_H_
#define INCLUDE_GEM_GLCLEARDEPTH_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglClearDepth
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glClearDepth( GLclampd depth)"
 */

class GEM_EXTERN GEMglClearDepth : public GemBase
{
	CPPEXTERN_HEADER(GEMglClearDepth, GemBase)

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
	  virtual void	depthMess(GLclampd);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[1];

	// static member functions
	  static void	 depthMessCallback (void*, GLclampd);
};
#endif // for header file
