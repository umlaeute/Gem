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

#ifndef INCLUDE_GEM_GLISTEXTURE_H_
#define INCLUDE_GEM_GLISTEXTURE_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglIsTexture
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glIsTexture( GLuint texture )"
 */

class GEM_EXTERN GEMglIsTexture : public GemBase
{
	CPPEXTERN_HEADER(GEMglIsTexture, GemBase)

	public:
	  // Constructor
	  GEMglIsTexture (t_floatarg);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglIsTexture ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLuint texture;		// VAR
	  virtual void	textureMess(t_float);	// FUN

	private:

	// we need some inlets
	  t_inlet *m_inlet;
	  t_outlet*m_outlet;

	// static member functions
	  static void	 textureMessCallback (void*, t_floatarg);
};
#endif // for header file
