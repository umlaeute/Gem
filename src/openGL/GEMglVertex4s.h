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

#ifndef INCLUDE_GEM_GLVERTEX4S_H_
#define INCLUDE_GEM_GLVERTEX4S_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglVertex4s
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glVertex4s( GLshort x, GLshort y, GLshort z, GLshort w)"
 */

class GEM_EXTERN GEMglVertex4s : public GemBase
{
	CPPEXTERN_HEADER(GEMglVertex4s, GemBase)

	public:
	  // Constructor
	  GEMglVertex4s (t_float, t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglVertex4s ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLshort	x;		// VAR
	  virtual void	xMess(t_float);	// FUN

	  GLshort	y;		// VAR
	  virtual void	yMess(t_float);	// FUN

	  GLshort	z;		// VAR
	  virtual void	zMess(t_float);	// FUN

	  GLshort	w;		// VAR
	  virtual void	wMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[4];

	// static member functions
	  static void	 xMessCallback (void*, t_floatarg);
	  static void	 yMessCallback (void*, t_floatarg);
	  static void	 zMessCallback (void*, t_floatarg);
	  static void	 wMessCallback (void*, t_floatarg);
};
#endif // for header file
