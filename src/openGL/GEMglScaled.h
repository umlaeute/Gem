/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glScaled(GLdouble x, GLdouble y, GLdouble z)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLSCALED_H_
#define INCLUDE_GEM_GLSCALED_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglScaled

    A Wrapper for the openGL-command "glScaled(GLdouble x, GLdouble y, GLdouble z)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglScaled : public GemBase
{
    CPPEXTERN_HEADER(GEMglScaled, GemBase)

    public:

        //////////
        // Constructor
        GEMglScaled (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglScaled();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLdouble x;		// VAR
	virtual void	xMess 	(double);		// FUN glScaled GLdouble

	GLdouble y;		// VAR
	virtual void	yMess 	(double);		// FUN glScaled GLdouble

	GLdouble z;		// VAR
	virtual void	zMess 	(double);		// FUN glScaled GLdouble


    private:

        //////////
        // Static member functions

	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glScaled
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glScaled
	static void	zMessCallback	(void*, t_floatarg);		// CALLBACK glScaled

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
