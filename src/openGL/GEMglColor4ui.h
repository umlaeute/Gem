/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glColor4ui(GLuint red, GLuint green, GLuint blue, GLuint alpha)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCOLOR4UI_H_
#define INCLUDE_GEM_GLCOLOR4UI_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglColor4ui

    A Wrapper for the openGL-command "glColor4ui(GLuint red, GLuint green, GLuint blue, GLuint alpha)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglColor4ui : public GemBase
{
    CPPEXTERN_HEADER(GEMglColor4ui, GemBase)

    public:

        //////////
        // Constructor
        GEMglColor4ui (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglColor4ui();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLuint red;		// VAR
	virtual void	redMess 	(int);		// FUN glColor4ui GLuint

	GLuint green;		// VAR
	virtual void	greenMess 	(int);		// FUN glColor4ui GLuint

	GLuint blue;		// VAR
	virtual void	blueMess 	(int);		// FUN glColor4ui GLuint

	GLuint alpha;		// VAR
	virtual void	alphaMess 	(int);		// FUN glColor4ui GLuint


    private:

        //////////
        // Static member functions

	static void	redMessCallback	(void*, t_floatarg);		// CALLBACK glColor4ui
	static void	greenMessCallback	(void*, t_floatarg);		// CALLBACK glColor4ui
	static void	blueMessCallback	(void*, t_floatarg);		// CALLBACK glColor4ui
	static void	alphaMessCallback	(void*, t_floatarg);		// CALLBACK glColor4ui

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
