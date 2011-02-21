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

#ifndef INCLUDE_GEM_GLENABLECLIENTSTATE_H_
#define INCLUDE_GEM_GLENABLECLIENTSTATE_H_

#include "GemGLBase.h"

/*
 CLASS
	GEMglEnableClientState
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glEnableClientState( GLenum array)"
 */

class GEM_EXTERN GEMglEnableClientState : public GemGLBase
{
	CPPEXTERN_HEADER(GEMglEnableClientState, GemGLBase)

	public:
	  // Constructor
	  GEMglEnableClientState (int, t_atom*); // CON

	protected:
	  // Destructor
	  virtual ~GEMglEnableClientState ();
          // check extensions
          virtual bool isRunnable(void);

	  // Do the rendering
	  virtual void	render (GemState *state);

	// variables
	  GLenum	array;		// VAR
	  virtual void	arrayMess(t_atom);	// FUN


	private:

	// we need some inlets
	  t_inlet *m_inlet[1];

	// static member functions
	  static void	 arrayMessCallback (void*,t_symbol*,int,t_atom*);
};
#endif // for header file
