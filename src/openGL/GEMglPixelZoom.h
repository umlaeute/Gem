/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glPixelZoom(GLfloat xfactor, GLfloat yfactor)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLPIXELZOOM_H_
#define INCLUDE_GEM_GLPIXELZOOM_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglPixelZoom

    A Wrapper for the openGL-command "glPixelZoom(GLfloat xfactor, GLfloat yfactor)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglPixelZoom : public GemBase
{
    CPPEXTERN_HEADER(GEMglPixelZoom, GemBase)

    public:

        //////////
        // Constructor
        GEMglPixelZoom (t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglPixelZoom();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLfloat xfactor;		// VAR
	virtual void	xfactorMess 	(t_float);		// FUN glPixelZoom GLfloat

	GLfloat yfactor;		// VAR
	virtual void	yfactorMess 	(t_float);		// FUN glPixelZoom GLfloat


    private:

        //////////
        // Static member functions

	static void	xfactorMessCallback	(void*, t_floatarg);		// CALLBACK glPixelZoom
	static void	yfactorMessCallback	(void*, t_floatarg);		// CALLBACK glPixelZoom

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
