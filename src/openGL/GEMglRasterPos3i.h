/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glRasterPos3i(GLint x, GLint y, GLint z)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLRASTERPOS3I_H_
#define INCLUDE_GEM_GLRASTERPOS3I_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglRasterPos3i

    A Wrapper for the openGL-command "glRasterPos3i(GLint x, GLint y, GLint z)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglRasterPos3i : public GemBase
{
    CPPEXTERN_HEADER(GEMglRasterPos3i, GemBase)

    public:

        //////////
        // Constructor
        GEMglRasterPos3i (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglRasterPos3i();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLint x;		// VAR
	virtual void	xMess 	(int);		// FUN glRasterPos3i GLint

	GLint y;		// VAR
	virtual void	yMess 	(int);		// FUN glRasterPos3i GLint

	GLint z;		// VAR
	virtual void	zMess 	(int);		// FUN glRasterPos3i GLint


    private:

        //////////
        // Static member functions

	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glRasterPos3i
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glRasterPos3i
	static void	zMessCallback	(void*, t_floatarg);		// CALLBACK glRasterPos3i

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
