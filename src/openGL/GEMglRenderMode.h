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

#ifndef INCLUDE_GEM_GLRENDERMODE_H_
#define INCLUDE_GEM_GLRENDERMODE_H_

#include "GemGLBase.h"

/*
 CLASS
	GEMglRenderMode
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glRenderMode( GLenum mode )"
 */

class GEM_EXTERN GEMglRenderMode : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglRenderMode, GemGLBase);

	public:
	// Constructor
	GEMglRenderMode (int, t_atom*); // CON

 protected:
	// Destructor
	virtual ~GEMglRenderMode ();
	// Do the rendering
	virtual void	render (GemState *state);
	
	// variables
	GLenum mode;		// VAR
	virtual void	modeMess(t_atom);	// FUN
	
	// we need some inlets
	t_inlet *m_inlet;
	t_outlet *m_outlet;
	
 private:
	
	// static member functions
	static void	 modeMessCallback (void*,t_symbol*,int,t_atom*);
};
#endif // for header file
