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

#ifndef INCLUDE_GEM_GLDISABLECLIENTSTATE_H_
#define INCLUDE_GEM_GLDISABLECLIENTSTATE_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglDisableClientState
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glDisableClientState( GLenum array)"
 */

class GEM_EXTERN GEMglDisableClientState : public GemBase
{
	CPPEXTERN_HEADER(GEMglDisableClientState, GemBase)

	public:
	  // Constructor
	  GEMglDisableClientState (t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglDisableClientState ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	array;		// VAR
	  virtual void	arrayMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[1];

	// static member functions
	  static void	 arrayMessCallback (void*, t_floatarg);
};
#endif // for header file
