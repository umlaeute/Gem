 /* ------------------------------------------------------------------
  * GEM - Graphics Environment for Multimedia
  *
  *  Copyright (c) 2004 tigital@mac.com
  *  For information on usage and redistribution, and for a DISCLAIMER
  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
  *
  * ------------------------------------------------------------------
  */

#ifndef INCLUDE_GEM_GLPROGRAMENVPARAMETER4DARB_H_
#define INCLUDE_GEM_GLPROGRAMENVPARAMETER4DARB_H_

#include "GemGLBase.h"

/*
 CLASS
	GEMglProgramEnvParameter4dARB
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glProgramEnvParameter4dARB( GLenum target, GLuint index, GLdouble x,
									GLdouble y, GLdouble z, GLdouble w)"
 */

class GEM_EXTERN GEMglProgramEnvParameter4dARB : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglProgramEnvParameter4dARB, GemGLBase);

	public:
	  // Constructor
	  GEMglProgramEnvParameter4dARB (int, t_atom*);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglProgramEnvParameter4dARB ();
          // check extensions
          virtual bool isRunnable(void);

	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	target;		// VAR
	  virtual void	targetMess(t_float);	// FUN

	  GLuint	index;		// VAR
	  virtual void	indexMess(t_float);	// FUN

	  GLdouble	m_x, m_y, m_z, m_w;		// VAR
	  virtual void	xMess(t_float);	// FUN
	  virtual void	yMess(t_float);	// FUN
	  virtual void	zMess(t_float);	// FUN
	  virtual void	wMess(t_float);	// FUN
	  
	  // this is protected because subclasses might want to use it
	  static void 	paramMessCallback(void *data, t_symbol*, int, t_atom*);

	private:

	// we need some inlets
	  t_inlet *m_inlet[6];

	// static member functions
	  static void	 targetMessCallback (void*, t_floatarg);
	  static void	 indexMessCallback (void*, t_floatarg);
	  static void	 xMessCallback (void*, t_floatarg);
	  static void	 yMessCallback (void*, t_floatarg);
	  static void	 zMessCallback (void*, t_floatarg);
	  static void	 wMessCallback (void*, t_floatarg);
};
#endif // for header file
