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

#ifndef INCLUDE_GEM_GLTEXCOORD1IV_H_
#define INCLUDE_GEM_GLTEXCOORD1IV_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglTexCoord1iv
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glTexCoord1iv( GLint* v)"
 */

class GEM_EXTERN GEMglTexCoord1iv : public GemBase
{
	CPPEXTERN_HEADER(GEMglTexCoord1iv, GemBase)

	public:
	  // Constructor
	  GEMglTexCoord1iv (t_float);	// CON
	protected:
	  // Destructor
	  virtual ~GEMglTexCoord1iv ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variable
	GLint	v[1];		// VAR
	virtual void	vMess(t_float);	// FUN

	private:

	// we need one inlet
	  t_inlet *m_inlet;

	// static member functions
         static void    vMessCallback (void*, t_floatarg);
};
#endif // for header file
