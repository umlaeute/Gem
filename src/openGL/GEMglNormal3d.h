/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glNormal3d(GLdouble nx, GLdouble ny, GLdouble nz)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLNORMAL3D_H_
#define INCLUDE_GEM_GLNORMAL3D_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglNormal3d

    A Wrapper for the openGL-command "glNormal3d(GLdouble nx, GLdouble ny, GLdouble nz)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglNormal3d : public GemBase
{
    CPPEXTERN_HEADER(GEMglNormal3d, GemBase)

    public:

        //////////
        // Constructor
        GEMglNormal3d (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglNormal3d();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLdouble nx;		// VAR
	virtual void	nxMess 	(double);		// FUN glNormal3d GLdouble

	GLdouble ny;		// VAR
	virtual void	nyMess 	(double);		// FUN glNormal3d GLdouble

	GLdouble nz;		// VAR
	virtual void	nzMess 	(double);		// FUN glNormal3d GLdouble


    private:

        //////////
        // Static member functions

	static void	nxMessCallback	(void*, t_floatarg);		// CALLBACK glNormal3d
	static void	nyMessCallback	(void*, t_floatarg);		// CALLBACK glNormal3d
	static void	nzMessCallback	(void*, t_floatarg);		// CALLBACK glNormal3d

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
