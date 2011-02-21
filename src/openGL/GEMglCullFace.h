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

#ifndef INCLUDE_GEM_GLCULLFACE_H_
#define INCLUDE_GEM_GLCULLFACE_H_

#include "GemGLBase.h"

/*
 CLASS
	GEMglCullFace
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glCullFace( GLenum mode)"
 */

class GEM_EXTERN GEMglCullFace : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglCullFace, GemGLBase)

	public:
	  // Constructor
	  GEMglCullFace (int, t_atom*); // CON

	protected:
	  // Destructor
	  virtual ~GEMglCullFace ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	mode;		// VAR
	  virtual void	modeMess(t_atom);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[1];

	// static member functions
	  static void	 modeMessCallback (void*,t_symbol*,int,t_atom*);
};
#endif // for header file
