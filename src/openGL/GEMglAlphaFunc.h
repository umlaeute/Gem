/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glAlphaFunc(GLenum func, GLclampf ref)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLALPHAFUNC_H_
#define INCLUDE_GEM_GLALPHAFUNC_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglAlphaFunc

    A Wrapper for the openGL-command "glAlphaFunc(GLenum func, GLclampf ref)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglAlphaFunc : public GemBase
{
    CPPEXTERN_HEADER(GEMglAlphaFunc, GemBase)

    public:

        //////////
        // Constructor
        GEMglAlphaFunc (int, t_atom*);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglAlphaFunc();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum func;		// VAR
	virtual void	funcMess 	(int, t_atom*);		// FUN glAlphaFunc GLenum

	GLclampf ref;		// VAR
	virtual void	refMess 	(t_float);		// FUN glAlphaFunc GLclampf


    private:

        //////////
        // Static member functions

	static void	funcMessCallback	(void*, t_symbol*,int,t_atom*);		// CALLBACK glAlphaFunc
	static void	refMessCallback	(void*, t_floatarg);		// CALLBACK glAlphaFunc

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
