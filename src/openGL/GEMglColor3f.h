/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glColor3f(GLfloat red, GLfloat green, GLfloat blue)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCOLOR3F_H_
#define INCLUDE_GEM_GLCOLOR3F_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglColor3f

    A Wrapper for the openGL-command "glColor3f(GLfloat red, GLfloat green, GLfloat blue)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglColor3f : public GemBase
{
    CPPEXTERN_HEADER(GEMglColor3f, GemBase)

    public:

        //////////
        // Constructor
        GEMglColor3f (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglColor3f();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLfloat red;		// VAR
	virtual void	redMess 	(t_float);		// FUN glColor3f GLfloat

	GLfloat green;		// VAR
	virtual void	greenMess 	(t_float);		// FUN glColor3f GLfloat

	GLfloat blue;		// VAR
	virtual void	blueMess 	(t_float);		// FUN glColor3f GLfloat


    private:

        //////////
        // Static member functions

	static void	redMessCallback	(void*, t_floatarg);		// CALLBACK glColor3f
	static void	greenMessCallback	(void*, t_floatarg);		// CALLBACK glColor3f
	static void	blueMessCallback	(void*, t_floatarg);		// CALLBACK glColor3f

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
