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

#ifndef INCLUDE_GEM_GLBINDTEXTURE_H_
#define INCLUDE_GEM_GLBINDTEXTURE_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglBindTexture
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glBindTexture( GLenum target, GLuint texture)"
 */

class GEM_EXTERN GEMglBindTexture : public GemBase
{
	CPPEXTERN_HEADER(GEMglBindTexture, GemBase)

	public:
	  // Constructor
	  GEMglBindTexture (t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglBindTexture ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	target;		// VAR
	  virtual void	targetMess(t_float);	// FUN

	  GLuint	texture;		// VAR
	  virtual void	textureMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[2];

	// static member functions
	  static void	 targetMessCallback (void*, t_floatarg);
	  static void	 textureMessCallback (void*, t_floatarg);
};
#endif // for header file
