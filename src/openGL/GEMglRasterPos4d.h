/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glRasterPos4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLRASTERPOS4D_H_
#define INCLUDE_GEM_GLRASTERPOS4D_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglRasterPos4d

    A Wrapper for the openGL-command "glRasterPos4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglRasterPos4d : public GemBase
{
    CPPEXTERN_HEADER(GEMglRasterPos4d, GemBase)

    public:

        //////////
        // Constructor
        GEMglRasterPos4d (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglRasterPos4d();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLdouble x;		// VAR
	virtual void	xMess 	(double);		// FUN glRasterPos4d GLdouble

	GLdouble y;		// VAR
	virtual void	yMess 	(double);		// FUN glRasterPos4d GLdouble

	GLdouble z;		// VAR
	virtual void	zMess 	(double);		// FUN glRasterPos4d GLdouble

	GLdouble w;		// VAR
	virtual void	wMess 	(double);		// FUN glRasterPos4d GLdouble


    private:

        //////////
        // Static member functions

	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glRasterPos4d
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glRasterPos4d
	static void	zMessCallback	(void*, t_floatarg);		// CALLBACK glRasterPos4d
	static void	wMessCallback	(void*, t_floatarg);		// CALLBACK glRasterPos4d

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
