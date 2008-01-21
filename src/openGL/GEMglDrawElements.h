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

#ifndef INCLUDE_GEM_GLDRAWELEMENTS_H_
#define INCLUDE_GEM_GLDRAWELEMENTS_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglDrawElements
 KEYWORDS
	openGL	1
 DESCRIPTION
	wrapper for the openGL-function
	"glDrawElements( GLenum mode, GLsizei count, GLenum type, GLvoid *indices)"
 */

class GEM_EXTERN GEMglDrawElements : public GemBase
{
	CPPEXTERN_HEADER(GEMglDrawElements, GemBase)

	public:
	  // Constructor
	  GEMglDrawElements (t_float, t_float, t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglDrawElements ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	mode;		// VAR
	  virtual void	modeMess(t_float);	// FUN

	  GLsizei	count;		// VAR
	  virtual void	countMess(t_float);	// FUN

	  GLenum	type;		// VAR
	  virtual void	typeMess(t_float);	// FUN

	  GLuint	*indices_ui;		// VAR
	  GLubyte	*indices_ub;		// VAR
	  GLushort	*indices_us;		// VAR
	  int           len;

	  virtual void	indicesMess(int, t_atom*);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[4];

	// static member functions
	  static void	 modeMessCallback (void*, t_floatarg);
	  static void	 countMessCallback (void*, t_floatarg);
	  static void	 typeMessCallback (void*, t_floatarg);
	  static void	 indicesMessCallback (void*, t_symbol*, int, t_atom*);
};
#endif // for header file
