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

#ifndef INCLUDE_GEM_GLTEXCOORD2F_H_
#define INCLUDE_GEM_GLTEXCOORD2F_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglTexCoord2f
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glTexCoord2f( GLfloat s, GLfloat t)"
 */

class GEM_EXTERN GEMglTexCoord2f : public GemBase
{
	CPPEXTERN_HEADER(GEMglTexCoord2f, GemBase)

	public:
	  // Constructor
	  GEMglTexCoord2f (t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglTexCoord2f ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLfloat	s;		// VAR
	  virtual void	sMess(t_float);	// FUN

	  GLfloat	t;		// VAR
	  virtual void	tMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[2];

	// static member functions
	  static void	 sMessCallback (void*, t_floatarg);
	  static void	 tMessCallback (void*, t_floatarg);
};
#endif // for header file
