/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glBlendFunc(GLenum sfactor, GLenum dfactor)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLBLENDFUNC_H_
#define INCLUDE_GEM_GLBLENDFUNC_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglBlendFunc

    A Wrapper for the openGL-command "glBlendFunc(GLenum sfactor, GLenum dfactor)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglBlendFunc : public GemBase
{
    CPPEXTERN_HEADER(GEMglBlendFunc, GemBase)

    public:

        //////////
        // Constructor
        GEMglBlendFunc (t_symbol*, t_symbol*);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglBlendFunc();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum sfactor;		// VAR
	virtual void	sfactorMess 	(int);		// FUN glBlendFunc GLenum

	GLenum dfactor;		// VAR
	virtual void	dfactorMess 	(int);		// FUN glBlendFunc GLenum


    private:

        //////////
        // Static member functions

	static void	sfactorMessCallback	(void*, t_symbol*);		// CALLBACK glBlendFunc
	static void	dfactorMessCallback	(void*, t_symbol*);		// CALLBACK glBlendFunc

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
