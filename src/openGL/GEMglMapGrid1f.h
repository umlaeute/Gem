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

#ifndef INCLUDE_GEM_GLMAPGRID1F_H_
#define INCLUDE_GEM_GLMAPGRID1F_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglMapGrid1f
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glMapGrid1f( GLint un, GLfloat u1, GLfloat u2)"
 */

class GEM_EXTERN GEMglMapGrid1f : public GemBase
{
	CPPEXTERN_HEADER(GEMglMapGrid1f, GemBase)

	public:
	  // Constructor
	  GEMglMapGrid1f (t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglMapGrid1f ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLint	un;		// VAR
	  virtual void	unMess(t_float);	// FUN

	  GLfloat	u1;		// VAR
	  virtual void	u1Mess(t_float);	// FUN

	  GLfloat	u2;		// VAR
	  virtual void	u2Mess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[3];

	// static member functions
	  static void	 unMessCallback (void*, t_floatarg);
	  static void	 u1MessCallback (void*, t_floatarg);
	  static void	 u2MessCallback (void*, t_floatarg);
};
#endif // for header file
