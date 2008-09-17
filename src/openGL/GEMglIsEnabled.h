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

#ifndef INCLUDE_GEM_GLISENABLED_H_
#define INCLUDE_GEM_GLISENABLED_H_

#include "GemGLBase.h"

/*
 CLASS
	GEMglIsEnabled
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glIsEnabled( glIsEnabled  GLenum cap )"
 */

class GEM_EXTERN GEMglIsEnabled : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglIsEnabled, GemGLBase)

    public:
  // Constructor
  GEMglIsEnabled (t_floatarg);	// CON

 protected:
  // Destructor
  virtual ~GEMglIsEnabled ();
          // check extensions
          virtual bool isRunnable(void);

  // Do the rendering
  virtual void	render (GemState *state);

  // variables
  GLenum cap;		// VAR
  virtual void	capMess(t_float);	// FUN

  // we need some inlets
  t_inlet *m_inlet;
  t_outlet*m_outlet;

 private:

  // static member functions
  static void	 capMessCallback (void*, t_floatarg);
};
#endif // for header file
