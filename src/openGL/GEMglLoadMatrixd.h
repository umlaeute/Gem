 /* ------------------------------------------------------------------
  * GEM - Graphics Environment for Multimedia
  *
  *  Copyright (c) 2004 tigital@mac.com
  *  For information on usage and redistribution, and for a DISCLAIMER
  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
  *
  * ------------------------------------------------------------------
  */

#ifndef INCLUDE_GEM_GLLOADMATRIXD_H_
#define INCLUDE_GEM_GLLOADMATRIXD_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglLoadMatrixd
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glLoadMatrixd( GLdouble matrix)"
 */

class GEM_EXTERN GEMglLoadMatrixd : public GemBase
{
	CPPEXTERN_HEADER(GEMglLoadMatrixd, GemBase)

	public:
	  // Constructor
	  GEMglLoadMatrixd (t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglLoadMatrixd ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	GLdouble	m_matrix[16];		// VAR
	  virtual void	matrixMess(int argc, t_atom*argv);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet;

	// static member functions
	  static void	 matrixMessCallback (void*, t_symbol*,int,t_atom*);
};
#endif // for header file
