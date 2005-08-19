 /* ------------------------------------------------------------------
  * GEM - Graphics Environment for Multimedia
  *
  *  Copyright (c) 2004-2005 tigital@mac.com
  *  For information on usage and redistribution, and for a DISCLAIMER
  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
  *
  * ------------------------------------------------------------------
  */

#ifndef INCLUDE_GEM_GLMULTITEXCOORD2FARB_H_
#define INCLUDE_GEM_GLMULTITEXCOORD2FARB_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglMultiTexCoord2fARB
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glMultiTexCoord2fARB( GLenum texUnit, GLfloat s, GLfloat t )"
 */

class GEM_EXTERN GEMglMultiTexCoord2fARB : public GemBase
{
	CPPEXTERN_HEADER(GEMglMultiTexCoord2fARB, GemBase)

	public:
	  // Constructor
	  GEMglMultiTexCoord2fARB (t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglMultiTexCoord2fARB ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	texUnit;		// VAR
	  virtual void	texUnitMess(t_float);	// FUN

	  GLfloat	s;		// VAR
	  virtual void	sMess(t_float);	// FUN

	  GLfloat	t;		// VAR
	  virtual void	tMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[3];

	// static member functions
	  static void	 texUnitMessCallback (void*, t_floatarg);
	  static void	 sMessCallback (void*, t_floatarg);
	  static void	 tMessCallback (void*, t_floatarg);
};
#endif // for header file

