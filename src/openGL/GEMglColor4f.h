/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCOLOR4F_H_
#define INCLUDE_GEM_GLCOLOR4F_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglColor4f

    A Wrapper for the openGL-command "glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglColor4f : public GemBase
{
    CPPEXTERN_HEADER(GEMglColor4f, GemBase)

    public:

        //////////
        // Constructor
        GEMglColor4f (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglColor4f();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLfloat red;		// VAR
	virtual void	redMess 	(t_float);		// FUN glColor4f GLfloat

	GLfloat green;		// VAR
	virtual void	greenMess 	(t_float);		// FUN glColor4f GLfloat

	GLfloat blue;		// VAR
	virtual void	blueMess 	(t_float);		// FUN glColor4f GLfloat

	GLfloat alpha;		// VAR
	virtual void	alphaMess 	(t_float);		// FUN glColor4f GLfloat


    private:

        //////////
        // Static member functions

	static void	redMessCallback	(void*, t_floatarg);		// CALLBACK glColor4f
	static void	greenMessCallback	(void*, t_floatarg);		// CALLBACK glColor4f
	static void	blueMessCallback	(void*, t_floatarg);		// CALLBACK glColor4f
	static void	alphaMessCallback	(void*, t_floatarg);		// CALLBACK glColor4f

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
