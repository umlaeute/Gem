/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glStencilOp(GLenum fail, GLenum zfail, GLenum zpass)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLSTENCILOP_H_
#define INCLUDE_GEM_GLSTENCILOP_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglStencilOp

    A Wrapper for the openGL-command "glStencilOp(GLenum fail, GLenum zfail, GLenum zpass)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglStencilOp : public GemBase
{
    CPPEXTERN_HEADER(GEMglStencilOp, GemBase)

    public:

        //////////
        // Constructor
        GEMglStencilOp (t_symbol*, t_symbol*, t_symbol*);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglStencilOp();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum fail;		// VAR
	virtual void	failMess 	(int);		// FUN glStencilOp GLenum

	GLenum zfail;		// VAR
	virtual void	zfailMess 	(int);		// FUN glStencilOp GLenum

	GLenum zpass;		// VAR
	virtual void	zpassMess 	(int);		// FUN glStencilOp GLenum


    private:

        //////////
        // Static member functions

	static void	failMessCallback	(void*, t_symbol*);		// CALLBACK glStencilOp
	static void	zfailMessCallback	(void*, t_symbol*);		// CALLBACK glStencilOp
	static void	zpassMessCallback	(void*, t_symbol*);		// CALLBACK glStencilOp

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
