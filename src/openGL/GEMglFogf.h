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

#ifndef INCLUDE_GEM_GLFOGF_H_
#define INCLUDE_GEM_GLFOGF_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglFogf
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glFogf( GLenum pname, GLfloat param)"
 */

class GEM_EXTERN GEMglFogf : public GemBase
{
	CPPEXTERN_HEADER(GEMglFogf, GemBase)

	public:
	  // Constructor
	  GEMglFogf (t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglFogf ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	pname;		// VAR
	  virtual void	pnameMess(t_float);	// FUN

	  GLfloat	param;		// VAR
	  virtual void	paramMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[2];

	// static member functions
	  static void	 pnameMessCallback (void*, t_floatarg);
	  static void	 paramMessCallback (void*, t_floatarg);
};
#endif // for header file
