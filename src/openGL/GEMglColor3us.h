/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glColor3us(GLushort red, GLushort green, GLushort blue)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCOLOR3US_H_
#define INCLUDE_GEM_GLCOLOR3US_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglColor3us

    A Wrapper for the openGL-command "glColor3us(GLushort red, GLushort green, GLushort blue)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglColor3us : public GemBase
{
    CPPEXTERN_HEADER(GEMglColor3us, GemBase)

    public:

        //////////
        // Constructor
        GEMglColor3us (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglColor3us();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLushort red;		// VAR
	virtual void	redMess 	(int);		// FUN glColor3us GLushort

	GLushort green;		// VAR
	virtual void	greenMess 	(int);		// FUN glColor3us GLushort

	GLushort blue;		// VAR
	virtual void	blueMess 	(int);		// FUN glColor3us GLushort


    private:

        //////////
        // Static member functions

	static void	redMessCallback	(void*, t_floatarg);		// CALLBACK glColor3us
	static void	greenMessCallback	(void*, t_floatarg);		// CALLBACK glColor3us
	static void	blueMessCallback	(void*, t_floatarg);		// CALLBACK glColor3us

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
