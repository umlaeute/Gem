/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLROTATED_H_
#define INCLUDE_GEM_GLROTATED_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglRotated

    A Wrapper for the openGL-command "glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglRotated : public GemBase
{
    CPPEXTERN_HEADER(GEMglRotated, GemBase)

    public:

        //////////
        // Constructor
        GEMglRotated (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglRotated();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLdouble angle;		// VAR
	virtual void	angleMess 	(double);		// FUN glRotated GLdouble

	GLdouble x;		// VAR
	virtual void	xMess 	(double);		// FUN glRotated GLdouble

	GLdouble y;		// VAR
	virtual void	yMess 	(double);		// FUN glRotated GLdouble

	GLdouble z;		// VAR
	virtual void	zMess 	(double);		// FUN glRotated GLdouble


    private:

        //////////
        // Static member functions

	static void	angleMessCallback	(void*, t_floatarg);		// CALLBACK glRotated
	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glRotated
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glRotated
	static void	zMessCallback	(void*, t_floatarg);		// CALLBACK glRotated

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
