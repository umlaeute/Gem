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

#ifndef INCLUDE_GEM_GLLINEWIDTH_H_
#define INCLUDE_GEM_GLLINEWIDTH_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglLineWidth
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glLineWidth( GLfloat width)"
 */

class GEM_EXTERN GEMglLineWidth : public GemBase
{
	CPPEXTERN_HEADER(GEMglLineWidth, GemBase)

	public:
	  // Constructor
	  GEMglLineWidth (t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglLineWidth ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLfloat	width;		// VAR
	  virtual void	widthMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[1];

	// static member functions
	  static void	 widthMessCallback (void*, t_floatarg);
};
#endif // for header file
