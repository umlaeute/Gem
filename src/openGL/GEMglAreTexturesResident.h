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

#ifndef INCLUDE_GEM_GLARETEXTURESRESIDENT_H_
#define INCLUDE_GEM_GLARETEXTURESRESIDENT_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglAreTexturesResident
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glAreTexturesResident( glAreTexturesResident  GLsizei n, GLuint *textures, GLboolean *residences )"
 */

class GEM_EXTERN GEMglAreTexturesResident : public GemBase
{
	CPPEXTERN_HEADER(GEMglAreTexturesResident, GemBase)

	public:
	  // Constructor
	  GEMglAreTexturesResident (int,t_atom*);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglAreTexturesResident ();
          // check extensions
          virtual bool isRunnable(void);

	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLsizei       n;
	  GLuint*	textures;		// VAR
	  virtual void	texturesMess(int,t_atom*);	// FUN

	  GLboolean*	residences;		// VAR

	  int           len;
	  t_atom*       m_buffer;

	private:

	// we need some inlets
	  t_inlet *m_inlet;
	  t_outlet *m_out1, *m_out2;

	// static member functions
	  static void	 texturesMessCallback (void*, t_symbol*,int,t_atom*);
};
#endif // for header file
