/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glColor3d(GLdouble red, GLdouble green, GLdouble blue)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCOLOR3D_H_
#define INCLUDE_GEM_GLCOLOR3D_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglColor3d

    A Wrapper for the openGL-command "glColor3d(GLdouble red, GLdouble green, GLdouble blue)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglColor3d : public GemBase
{
    CPPEXTERN_HEADER(GEMglColor3d, GemBase)

    public:

        //////////
        // Constructor
        GEMglColor3d (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglColor3d();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLdouble red;		// VAR
	virtual void	redMess 	(double);		// FUN glColor3d GLdouble

	GLdouble green;		// VAR
	virtual void	greenMess 	(double);		// FUN glColor3d GLdouble

	GLdouble blue;		// VAR
	virtual void	blueMess 	(double);		// FUN glColor3d GLdouble


    private:

        //////////
        // Static member functions

	static void	redMessCallback	(void*, t_floatarg);		// CALLBACK glColor3d
	static void	greenMessCallback	(void*, t_floatarg);		// CALLBACK glColor3d
	static void	blueMessCallback	(void*, t_floatarg);		// CALLBACK glColor3d

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
