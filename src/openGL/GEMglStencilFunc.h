/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glStencilFunc(GLenum func, GLint ref, GLuint mask)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLSTENCILFUNC_H_
#define INCLUDE_GEM_GLSTENCILFUNC_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglStencilFunc

    A Wrapper for the openGL-command "glStencilFunc(GLenum func, GLint ref, GLuint mask)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglStencilFunc : public GemBase
{
    CPPEXTERN_HEADER(GEMglStencilFunc, GemBase)

    public:

        //////////
        // Constructor
        GEMglStencilFunc (t_symbol*, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglStencilFunc();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum func;		// VAR
	virtual void	funcMess 	(int);		// FUN glStencilFunc GLenum

	GLint ref;		// VAR
	virtual void	refMess 	(int);		// FUN glStencilFunc GLint

	GLuint mask;		// VAR
	virtual void	maskMess 	(int);		// FUN glStencilFunc GLuint


    private:

        //////////
        // Static member functions

	static void	funcMessCallback	(void*, t_symbol*);		// CALLBACK glStencilFunc
	static void	refMessCallback	(void*, t_floatarg);		// CALLBACK glStencilFunc
	static void	maskMessCallback	(void*, t_floatarg);		// CALLBACK glStencilFunc

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
