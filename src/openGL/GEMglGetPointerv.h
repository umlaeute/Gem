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

#ifndef INCLUDE_GEM_GLGETPOINTERV_H_
#define INCLUDE_GEM_GLGETPOINTERV_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglGetPointerv
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glGetPointerv( GLenum pname, GLvoid* *params)"
 */

class GEM_EXTERN GEMglGetPointerv : public GemBase
{
	CPPEXTERN_HEADER(GEMglGetPointerv, GemBase)

	public:
	  // Constructor
	  GEMglGetPointerv (t_floatarg);	// CON
	protected:
	  // Destructor
	  virtual ~GEMglGetPointerv ();
          // check extensions
          virtual bool isRunnable(void);

	  // Do the rendering
	  virtual void	render (GemState *state);

	// variable
	GLenum pname;
	virtual void	pnameMess(t_floatarg);	// FUN
	GLvoid **params;		// VAR

	private:

	// we need one inlet
	  t_inlet *m_inlet;

	// static member functions
         static void    pnameMessCallback (void*,t_floatarg);
};
#endif // for header file
