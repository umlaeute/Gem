/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glClearStencil(GLint s)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCLEARSTENCIL_H_
#define INCLUDE_GEM_GLCLEARSTENCIL_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglClearStencil

    A Wrapper for the openGL-command "glClearStencil(GLint s)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglClearStencil : public GemBase
{
    CPPEXTERN_HEADER(GEMglClearStencil, GemBase)

    public:

        //////////
        // Constructor
        GEMglClearStencil (t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglClearStencil();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLint s;		// VAR
	virtual void	sMess 	(int);		// FUN glClearStencil GLint


    private:

        //////////
        // Static member functions

	static void	sMessCallback	(void*, t_floatarg);		// CALLBACK glClearStencil

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
