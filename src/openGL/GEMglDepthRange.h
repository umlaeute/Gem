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

#ifndef INCLUDE_GEM_GLDEPTHRANGE_H_
#define INCLUDE_GEM_GLDEPTHRANGE_H_

#include "Base/GemGLBase.h"

/*
 CLASS
	GEMglDepthRange
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glDepthRange( GLclampd near_val, GLclampd far_val)"
 */

class GEM_EXTERN GEMglDepthRange : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglDepthRange, GemGLBase);

	public:
	  // Constructor
	  GEMglDepthRange (t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglDepthRange ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLclampd	near_val;		// VAR
	  virtual void	near_valMess(GLclampd);	// FUN

	  GLclampd	far_val;		// VAR
	  virtual void	far_valMess(GLclampd);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[2];

	// static member functions
	  static void	 near_valMessCallback (void*, GLclampd);
	  static void	 far_valMessCallback (void*, GLclampd);
};
#endif // for header file
