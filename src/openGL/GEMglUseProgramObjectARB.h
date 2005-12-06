 /* ------------------------------------------------------------------
  * GEM - Graphics Environment for Multimedia
  *
  *  Copyright (c) 2005 tigital@mac.com
  *  For information on usage and redistribution, and for a DISCLAIMER
  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
  * ------------------------------------------------------------------
  */

#ifndef INCLUDE_GEM_GLUSEPROGRAMOBJECTARB_H_
#define INCLUDE_GEM_GLUSEPROGRAMOBJECTARB_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

#ifdef __APPLE__
# define t_GLshaderObj GLhandleARB*
#else
# define t_GLshaderObj GLhandleARB
#endif

/*
 CLASS
	GEMglUseProgramObjectARB
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glUseProgramObjectARB( GLenum program )"
 */

class GEM_EXTERN GEMglUseProgramObjectARB : public GemBase
{
	CPPEXTERN_HEADER(GEMglUseProgramObjectARB, GemBase)

	public:
	  // Constructor
	  GEMglUseProgramObjectARB ();	// CON

	protected:
	  // Destructor
	  virtual ~GEMglUseProgramObjectARB ();
	  
	  // Do the rendering
	  virtual void	render (GemState *state);
	  
	  // clean up the postrendering
	  virtual void	postrender (GemState *state);

	// variables
	  t_GLshaderObj	m_program;		// VAR
	  virtual void	programMess(int);

	private:

	// we need some inlets
	  t_inlet *m_inlet;

	// static member functions
	  static void	 programMessCallback (void*, t_floatarg);
};
#endif // for header file
