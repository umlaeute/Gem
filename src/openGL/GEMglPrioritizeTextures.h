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

#ifndef INCLUDE_GEM_GLPRIORITIZETEXTURES_H_
#define INCLUDE_GEM_GLPRIORITIZETEXTURES_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglPrioritizeTextures
 KEYWORDS
	openGL	1
 DESCRIPTION
	wrapper for the openGL-function
	"glPrioritizeTextures( GLsizei n, GLuint *textures, GLclampf *priorities)"
 */

class GEM_EXTERN GEMglPrioritizeTextures : public GemBase
{
	CPPEXTERN_HEADER(GEMglPrioritizeTextures, GemBase)

	public:
	  // Constructor
	  GEMglPrioritizeTextures (t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglPrioritizeTextures ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLsizei	n;		// VAR
	  virtual void	nMess(t_float);	// FUN

	  GLuint*	textures;		// VAR
	  int t_len;
	  virtual void	texturesMess(int,t_atom*);	// FUN

	  GLclampf*	priorities;		// VAR
	  int p_len;
	  virtual void	prioritiesMess(int,t_atom*);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[3];

	// static member functions
	  static void	 nMessCallback (void*, t_floatarg);
	  static void	 texturesMessCallback (void*, int, t_atom*);
	  static void	 prioritiesMessCallback (void*, int,t_atom*);
};
#endif // for header file
