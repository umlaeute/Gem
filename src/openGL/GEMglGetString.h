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

#ifndef INCLUDE_GEM_GLGETSTRING_H_
#define INCLUDE_GEM_GLGETSTRING_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*
 CLASS
	GEMglGetString
 KEYWORDS
	openGL	0
 DESCRIPTION
	wrapper for the openGL-function
	"glGetString( glGetString  GLenum name )"
 */

class GEM_EXTERN GEMglGetString : public GemBase
{
  CPPEXTERN_HEADER(GEMglGetString, GemBase)

    public:
  // Constructor
  GEMglGetString (t_floatarg);	// CON
  
 protected:
  // Destructor
  virtual ~GEMglGetString ();
  // Do the rendering
  virtual void	render (GemState *state);
  
  // variables
  GLenum name;		// VAR
  virtual void	nameMess(t_float);	// FUN
  
  // we need some inl/outets
  t_inlet *m_inlet;
  t_outlet*m_outlet;

 private:

  // static member functions
  static void	 nameMessCallback (void*, t_floatarg);
};
#endif // for header file
