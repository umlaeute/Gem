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

#ifndef INCLUDE_GEM_GLEVALMESH1_H_
#define INCLUDE_GEM_GLEVALMESH1_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglEvalMesh1
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glEvalMesh1( GLenum mode, GLint i1, GLint i2)"
 */

class GEM_EXTERN GEMglEvalMesh1 : public GemBase
{
	CPPEXTERN_HEADER(GEMglEvalMesh1, GemBase)

	public:
	  // Constructor
	  GEMglEvalMesh1 (t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglEvalMesh1 ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	mode;		// VAR
	  virtual void	modeMess(t_float);	// FUN

	  GLint	i1;		// VAR
	  virtual void	i1Mess(t_float);	// FUN

	  GLint	i2;		// VAR
	  virtual void	i2Mess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[3];

	// static member functions
	  static void	 modeMessCallback (void*, t_floatarg);
	  static void	 i1MessCallback (void*, t_floatarg);
	  static void	 i2MessCallback (void*, t_floatarg);
};
#endif // for header file
