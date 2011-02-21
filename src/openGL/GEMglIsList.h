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

#ifndef INCLUDE_GEM_GLISLIST_H_
#define INCLUDE_GEM_GLISLIST_H_

#include "GemGLBase.h"

/*
 CLASS
	GEMglIsList
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glIsList( GLuint list )"
 */

class GEM_EXTERN GEMglIsList : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglIsList, GemGLBase)

	public:
	  // Constructor
	  GEMglIsList (t_floatarg);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglIsList ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLuint list;		// VAR
	  virtual void	listMess(t_float);	// FUN

	private:

	// we need some inlets
	  t_inlet *m_inlet;
	  t_outlet*m_outlet;

	// static member functions
	  static void	 listMessCallback (void*, t_floatarg);
};
#endif // for header file
