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

#ifndef INCLUDE_GEM_GLDELETETEXTURES_H_
#define INCLUDE_GEM_GLDELETETEXTURES_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglDeleteTextures
 KEYWORDS
	openGL	1
 DESCRIPTION
	wrapper for the openGL-function
	"glDeleteTextures( GLsizei n, GLuint *textures)"
 */

class GEM_EXTERN GEMglDeleteTextures : public GemBase
{
	CPPEXTERN_HEADER(GEMglDeleteTextures, GemBase)

	public:
	  // Constructor
	  GEMglDeleteTextures (int,t_atom*);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglDeleteTextures ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLsizei	n;		// VAR
	  GLuint*	textures;		// VAR
	  virtual void	texturesMess(int,t_atom*);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet;

	// static member functions
	  static void	 texturesMessCallback (void*, int,t_atom*);
};
#endif // for header file
