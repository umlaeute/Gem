/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCOLOR4UB_H_
#define INCLUDE_GEM_GLCOLOR4UB_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglColor4ub

    A Wrapper for the openGL-command "glColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglColor4ub : public GemBase
{
    CPPEXTERN_HEADER(GEMglColor4ub, GemBase)

    public:

        //////////
        // Constructor
        GEMglColor4ub (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglColor4ub();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLubyte red;		// VAR
	virtual void	redMess 	(int);		// FUN glColor4ub GLubyte

	GLubyte green;		// VAR
	virtual void	greenMess 	(int);		// FUN glColor4ub GLubyte

	GLubyte blue;		// VAR
	virtual void	blueMess 	(int);		// FUN glColor4ub GLubyte

	GLubyte alpha;		// VAR
	virtual void	alphaMess 	(int);		// FUN glColor4ub GLubyte


    private:

        //////////
        // Static member functions

	static void	redMessCallback	(void*, t_floatarg);		// CALLBACK glColor4ub
	static void	greenMessCallback	(void*, t_floatarg);		// CALLBACK glColor4ub
	static void	blueMessCallback	(void*, t_floatarg);		// CALLBACK glColor4ub
	static void	alphaMessCallback	(void*, t_floatarg);		// CALLBACK glColor4ub

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
