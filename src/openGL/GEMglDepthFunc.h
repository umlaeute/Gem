/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glDepthFunc(GLenum func)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLDEPTHFUNC_H_
#define INCLUDE_GEM_GLDEPTHFUNC_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglDepthFunc

    A Wrapper for the openGL-command "glDepthFunc(GLenum func)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglDepthFunc : public GemBase
{
    CPPEXTERN_HEADER(GEMglDepthFunc, GemBase)

    public:

        //////////
        // Constructor
        GEMglDepthFunc (t_symbol*);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglDepthFunc();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum func;		// VAR
	virtual void	funcMess 	(int);		// FUN glDepthFunc GLenum


    private:

        //////////
        // Static member functions

	static void	funcMessCallback	(void*, t_symbol*);		// CALLBACK glDepthFunc

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
