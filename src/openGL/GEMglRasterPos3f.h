/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glRasterPos3f(GLfloat x, GLfloat y, GLfloat z)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLRASTERPOS3F_H_
#define INCLUDE_GEM_GLRASTERPOS3F_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglRasterPos3f

    A Wrapper for the openGL-command "glRasterPos3f(GLfloat x, GLfloat y, GLfloat z)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglRasterPos3f : public GemBase
{
    CPPEXTERN_HEADER(GEMglRasterPos3f, GemBase)

    public:

        //////////
        // Constructor
        GEMglRasterPos3f (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglRasterPos3f();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLfloat x;		// VAR
	virtual void	xMess 	(t_float);		// FUN glRasterPos3f GLfloat

	GLfloat y;		// VAR
	virtual void	yMess 	(t_float);		// FUN glRasterPos3f GLfloat

	GLfloat z;		// VAR
	virtual void	zMess 	(t_float);		// FUN glRasterPos3f GLfloat


    private:

        //////////
        // Static member functions

	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glRasterPos3f
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glRasterPos3f
	static void	zMessCallback	(void*, t_floatarg);		// CALLBACK glRasterPos3f

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
