/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glVertex3d(GLdouble x, GLdouble y, GLdouble z)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLVERTEX3D_H_
#define INCLUDE_GEM_GLVERTEX3D_H_

#include "Base/GemGLUtil.h"
//#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglVertex3d

    A Wrapper for the openGL-command "glVertex3d(GLdouble x, GLdouble y, GLdouble z)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglVertex3d : public GemBase
{
    CPPEXTERN_HEADER(GEMglVertex3d, GemBase)

    public:

        //////////
        // Constructor
        GEMglVertex3d (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglVertex3d();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLdouble x;		// VAR
	virtual void	xMess 	(t_float);		// FUN glVertex3d GLdouble

	GLdouble y;		// VAR
	virtual void	yMess 	(t_float);		// FUN glVertex3d GLdouble

	GLdouble z;		// VAR
	virtual void	zMess 	(t_float);		// FUN glVertex3d GLdouble


    private:

        //////////
        // Static member functions

	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glVertex3d
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glVertex3d
	static void	zMessCallback	(void*, t_floatarg);		// CALLBACK glVertex3d

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
