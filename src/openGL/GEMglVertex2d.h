/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glVertex2d(GLdouble x, GLdouble y)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLVERTEX2D_H_
#define INCLUDE_GEM_GLVERTEX2D_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglVertex2d

    A Wrapper for the openGL-command "glVertex2d(GLdouble x, GLdouble y)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglVertex2d : public GemBase
{
    CPPEXTERN_HEADER(GEMglVertex2d, GemBase)

    public:

        //////////
        // Constructor
        GEMglVertex2d (t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglVertex2d();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLdouble x;		// VAR
	virtual void	xMess 	(double);		// FUN glVertex2d GLdouble

	GLdouble y;		// VAR
	virtual void	yMess 	(double);		// FUN glVertex2d GLdouble


    private:

        //////////
        // Static member functions

	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glVertex2d
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glVertex2d

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
