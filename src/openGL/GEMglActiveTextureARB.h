 /* ------------------------------------------------------------------
  * GEM - Graphics Environment for Multimedia
  *
  *  Copyright (c) 2004-2005 tigital@mac.com
  *  For information on usage and redistribution, and for a DISCLAIMER
  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
  * ------------------------------------------------------------------
  */

#ifndef INCLUDE_GEM_GLACTIVETEXTUREARB_H_
#define INCLUDE_GEM_GLACTIVETEXTUREARB_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglActiveTextureARB
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glActiveTextureARB( GLenum target )"
 */

class GEM_EXTERN GEMglActiveTextureARB : public GemBase
{
	CPPEXTERN_HEADER(GEMglActiveTextureARB, GemBase)

	public:
	  // Constructor
	  GEMglActiveTextureARB (t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglActiveTextureARB ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	texUnit;		// VAR
	  virtual void	texUnitMess(t_float);	// FUN

	private:

	// we need some inlets
	  t_inlet *m_inlet[1];

	// static member functions
	  static void	 texUnitMessCallback (void*, t_floatarg);
};
#endif // for header file
