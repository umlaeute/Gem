/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLNORMAL3F_H_
#define INCLUDE_GEM_GLNORMAL3F_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglNormal3f

    A Wrapper for the openGL-command "glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglNormal3f : public GemBase
{
    CPPEXTERN_HEADER(GEMglNormal3f, GemBase)

    public:

        //////////
        // Constructor
        GEMglNormal3f (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglNormal3f();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLfloat nx;		// VAR
	virtual void	nxMess 	(t_float);		// FUN glNormal3f GLfloat

	GLfloat ny;		// VAR
	virtual void	nyMess 	(t_float);		// FUN glNormal3f GLfloat

	GLfloat nz;		// VAR
	virtual void	nzMess 	(t_float);		// FUN glNormal3f GLfloat


    private:

        //////////
        // Static member functions

	static void	nxMessCallback	(void*, t_floatarg);		// CALLBACK glNormal3f
	static void	nyMessCallback	(void*, t_floatarg);		// CALLBACK glNormal3f
	static void	nzMessCallback	(void*, t_floatarg);		// CALLBACK glNormal3f

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
