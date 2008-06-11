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

#ifndef INCLUDE_GEM_GLSELECTBUFFER_H_
#define INCLUDE_GEM_GLSELECTBUFFER_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglSelectBuffer
 KEYWORDS
	openGL	1
 DESCRIPTION
	wrapper for the openGL-function
	"glSelectBuffer( GLsizei size, GLuint *buffer)"
 */

class GEM_EXTERN GEMglSelectBuffer : public GemBase
{
	CPPEXTERN_HEADER(GEMglSelectBuffer, GemBase)

	public:
	  // Constructor
	  GEMglSelectBuffer (t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglSelectBuffer ();
          // check extensions
          virtual bool isRunnable(void);

	  // Do the rendering
	  virtual void	render (GemState *state);
	  virtual void	postrender (GemState *state);

	// variables
	  GLsizei	size;		// VAR
	  virtual void	sizeMess(t_float);	// FUN

	  int len;
	  GLuint*	buffer;		// VAR

	private:

	  // we need some inlets
	  t_inlet *m_inlet;
	  t_outlet*m_bufout;

	// static member functions
	  static void	 sizeMessCallback (void*, t_floatarg);
};
#endif // for header file
