/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glRasterPos2f(GLfloat x, GLfloat y)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLRASTERPOS2F_H_
#define INCLUDE_GEM_GLRASTERPOS2F_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglRasterPos2f

    A Wrapper for the openGL-command "glRasterPos2f(GLfloat x, GLfloat y)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglRasterPos2f : public GemBase
{
    CPPEXTERN_HEADER(GEMglRasterPos2f, GemBase)

    public:

        //////////
        // Constructor
        GEMglRasterPos2f (t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglRasterPos2f();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLfloat x;		// VAR
	virtual void	xMess 	(t_float);		// FUN glRasterPos2f GLfloat

	GLfloat y;		// VAR
	virtual void	yMess 	(t_float);		// FUN glRasterPos2f GLfloat


    private:

        //////////
        // Static member functions

	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glRasterPos2f
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glRasterPos2f

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
