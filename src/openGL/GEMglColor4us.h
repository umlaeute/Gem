/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glColor4us(GLushort red, GLushort green, GLushort blue, GLushort alpha)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCOLOR4US_H_
#define INCLUDE_GEM_GLCOLOR4US_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglColor4us

    A Wrapper for the openGL-command "glColor4us(GLushort red, GLushort green, GLushort blue, GLushort alpha)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglColor4us : public GemBase
{
    CPPEXTERN_HEADER(GEMglColor4us, GemBase)

    public:

        //////////
        // Constructor
        GEMglColor4us (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglColor4us();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLushort red;		// VAR
	virtual void	redMess 	(int);		// FUN glColor4us GLushort

	GLushort green;		// VAR
	virtual void	greenMess 	(int);		// FUN glColor4us GLushort

	GLushort blue;		// VAR
	virtual void	blueMess 	(int);		// FUN glColor4us GLushort

	GLushort alpha;		// VAR
	virtual void	alphaMess 	(int);		// FUN glColor4us GLushort


    private:

        //////////
        // Static member functions

	static void	redMessCallback	(void*, t_floatarg);		// CALLBACK glColor4us
	static void	greenMessCallback	(void*, t_floatarg);		// CALLBACK glColor4us
	static void	blueMessCallback	(void*, t_floatarg);		// CALLBACK glColor4us
	static void	alphaMessCallback	(void*, t_floatarg);		// CALLBACK glColor4us

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
