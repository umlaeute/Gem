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

#ifndef INCLUDE_GEM_GLFEEDBACKBUFFER_H_
#define INCLUDE_GEM_GLFEEDBACKBUFFER_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglFeedbackBuffer
 KEYWORDS
	openGL	1
 DESCRIPTION
	wrapper for the openGL-function
	"glFeedbackBuffer( GLsizei size, GLenum type, GLfloat *buffer)"
 */

class GEM_EXTERN GEMglFeedbackBuffer : public GemBase
{
	CPPEXTERN_HEADER(GEMglFeedbackBuffer, GemBase)

	public:
	  // Constructor
	  GEMglFeedbackBuffer (t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglFeedbackBuffer ();
          // check extensions
          virtual bool isRunnable(void);

	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLsizei	size;		// VAR
	  virtual void	sizeMess(t_float);	// FUN

	  GLenum	type;		// VAR
	  virtual void	typeMess(t_float);	// FUN

	  GLsizei       len;
	  GLfloat      *buffer;
	private:

	// we need some inlets
	  t_inlet *m_inlet[2];

	// static member functions
	  static void	 sizeMessCallback (void*, t_floatarg);
	  static void	 typeMessCallback (void*, t_floatarg);
};
#endif // for header file
