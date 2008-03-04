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

#ifndef INCLUDE_GEM_GLMAP2D_H_
#define INCLUDE_GEM_GLMAP2D_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglMap2d
 KEYWORDS
	openGL	1
 DESCRIPTION
	wrapper for the openGL-function
	"glMap2d( GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, GLdouble *points)"
 */

class GEM_EXTERN GEMglMap2d : public GemBase
{
	CPPEXTERN_HEADER(GEMglMap2d, GemBase)

	public:
	  // Constructor
	  GEMglMap2d (int,t_atom*);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglMap2d ();
          // check extensions
          virtual bool isRunnable(void);

	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	target;		// VAR
	  virtual void	targetMess(t_float);	// FUN

	  GLdouble	u1;		// VAR
	  virtual void	u1Mess(t_float);	// FUN

	  GLdouble	u2;		// VAR
	  virtual void	u2Mess(t_float);	// FUN

	  GLint	ustride;		// VAR
	  virtual void	ustrideMess(t_float);	// FUN

	  GLint	uorder;		// VAR
	  virtual void	uorderMess(t_float);	// FUN

	  GLdouble	v1;		// VAR
	  virtual void	v1Mess(t_float);	// FUN

	  GLdouble	v2;		// VAR
	  virtual void	v2Mess(t_float);	// FUN

	  GLint	vstride;		// VAR
	  virtual void	vstrideMess(t_float);	// FUN

	  GLint	vorder;		// VAR
	  virtual void	vorderMess(t_float);	// FUN

	  GLdouble*	points;		// VAR
	  int len;
	  virtual void	pointsMess(int,t_atom*);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[10];

	// static member functions
	  static void	 targetMessCallback (void*, t_floatarg);
	  static void	 u1MessCallback (void*, t_floatarg);
	  static void	 u2MessCallback (void*, t_floatarg);
	  static void	 ustrideMessCallback (void*, t_floatarg);
	  static void	 uorderMessCallback (void*, t_floatarg);
	  static void	 v1MessCallback (void*, t_floatarg);
	  static void	 v2MessCallback (void*, t_floatarg);
	  static void	 vstrideMessCallback (void*, t_floatarg);
	  static void	 vorderMessCallback (void*, t_floatarg);
	  static void	 pointsMessCallback (void*, t_symbol*, int,t_atom*);
};
#endif // for header file
