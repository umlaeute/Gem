/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glStencilMask(GLuint mask)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLSTENCILMASK_H_
#define INCLUDE_GEM_GLSTENCILMASK_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglStencilMask

    A Wrapper for the openGL-command "glStencilMask(GLuint mask)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglStencilMask : public GemBase
{
    CPPEXTERN_HEADER(GEMglStencilMask, GemBase)

    public:

        //////////
        // Constructor
        GEMglStencilMask (t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglStencilMask();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLuint mask;		// VAR
	virtual void	maskMess 	(int);		// FUN glStencilMask GLuint


    private:

        //////////
        // Static member functions

	static void	maskMessCallback	(void*, t_floatarg);		// CALLBACK glStencilMask

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
