/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glColor4d(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCOLOR4D_H_
#define INCLUDE_GEM_GLCOLOR4D_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglColor4d

    A Wrapper for the openGL-command "glColor4d(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglColor4d : public GemBase
{
    CPPEXTERN_HEADER(GEMglColor4d, GemBase)

    public:

        //////////
        // Constructor
        GEMglColor4d (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglColor4d();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLdouble red;		// VAR
	virtual void	redMess 	(double);		// FUN glColor4d GLdouble

	GLdouble green;		// VAR
	virtual void	greenMess 	(double);		// FUN glColor4d GLdouble

	GLdouble blue;		// VAR
	virtual void	blueMess 	(double);		// FUN glColor4d GLdouble

	GLdouble alpha;		// VAR
	virtual void	alphaMess 	(double);		// FUN glColor4d GLdouble


    private:

        //////////
        // Static member functions

	static void	redMessCallback	(void*, t_floatarg);		// CALLBACK glColor4d
	static void	greenMessCallback	(void*, t_floatarg);		// CALLBACK glColor4d
	static void	blueMessCallback	(void*, t_floatarg);		// CALLBACK glColor4d
	static void	alphaMessCallback	(void*, t_floatarg);		// CALLBACK glColor4d

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
