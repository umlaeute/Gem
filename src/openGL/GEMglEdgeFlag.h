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

#ifndef INCLUDE_GEM_GLEDGEFLAG_H_
#define INCLUDE_GEM_GLEDGEFLAG_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglEdgeFlag
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glEdgeFlag( GLboolean flag)"
 */

class GEM_EXTERN GEMglEdgeFlag : public GemBase
{
	CPPEXTERN_HEADER(GEMglEdgeFlag, GemBase)

	public:
	  // Constructor
	  GEMglEdgeFlag (t_float);	// CON

	protected:
	  // Destructor
	  virtual ~GEMglEdgeFlag ();
	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLboolean	flag;		// VAR
	  virtual void	flagMess(t_float);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[1];

	// static member functions
	  static void	 flagMessCallback (void*, t_floatarg);
};
#endif // for header file
