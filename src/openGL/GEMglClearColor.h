/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCLEARCOLOR_H_
#define INCLUDE_GEM_GLCLEARCOLOR_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglClearColor

    A Wrapper for the openGL-command "glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglClearColor : public GemBase
{
    CPPEXTERN_HEADER(GEMglClearColor, GemBase)

    public:

        //////////
        // Constructor
        GEMglClearColor (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglClearColor();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLclampf red;		// VAR
	virtual void	redMess 	(t_float);		// FUN glClearColor GLclampf

	GLclampf green;		// VAR
	virtual void	greenMess 	(t_float);		// FUN glClearColor GLclampf

	GLclampf blue;		// VAR
	virtual void	blueMess 	(t_float);		// FUN glClearColor GLclampf

	GLclampf alpha;		// VAR
	virtual void	alphaMess 	(t_float);		// FUN glClearColor GLclampf


    private:

        //////////
        // Static member functions

	static void	redMessCallback	(void*, t_floatarg);		// CALLBACK glClearColor
	static void	greenMessCallback	(void*, t_floatarg);		// CALLBACK glClearColor
	static void	blueMessCallback	(void*, t_floatarg);		// CALLBACK glClearColor
	static void	alphaMessCallback	(void*, t_floatarg);		// CALLBACK glClearColor

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
