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

#ifndef INCLUDE_GEM_GLFOGIV_H_
#define INCLUDE_GEM_GLFOGIV_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglFogiv
 KEYWORDS
	openGL	1
 DESCRIPTION
	wrapper for the openGL-function
	"glFogiv(GLenum pname, GLint *params)"
 */

#define FOG_ARRAY_LENGTH 4


class GEM_EXTERN GEMglFogiv : public GemBase
{
	CPPEXTERN_HEADER(GEMglFogiv, GemBase)

	public:
	  // Constructor
	  GEMglFogiv (int,t_atom*);	// CON
	protected:
	  // Destructor
	  virtual ~GEMglFogiv ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variable
	GLenum          pname;
	virtual void	pnameMess(t_float);	// FUN

	GLint    	params[FOG_ARRAY_LENGTH];		// VAR
	virtual void	paramsMess(int, t_atom*);	// FUN

	private:

	// we need one inlet
	t_inlet *m_inlet[2];

	// static member functions
         static void    pnameMessCallback (void*, t_float);
         static void    paramsMessCallback (void*, int, t_atom*);
};
#endif // for header file
