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

#ifndef INCLUDE_GEM_GLGENTEXTURES_H_
#define INCLUDE_GEM_GLGENTEXTURES_H_

#include "GemGLBase.h"

/*
 CLASS
	GEMglGenTextures
 KEYWORDS
	openGL	1
 DESCRIPTION
	wrapper for the openGL-function
	"glGenTextures( GLsizei n, GLuint *textures)"
 */

class GEM_EXTERN GEMglGenTextures : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglGenTextures, GemGLBase)

	public:
	  // Constructor
	  GEMglGenTextures (int,t_atom*);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglGenTextures ();
          // check extensions
          virtual bool isRunnable(void);

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
	  static void	 texturesMessCallback (void*, t_symbol*, int, t_atom*);
};
#endif // for header file
