/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glScalef(GLfloat x, GLfloat y, GLfloat z)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLSCALEF_H_
#define INCLUDE_GEM_GLSCALEF_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglScalef

    A Wrapper for the openGL-command "glScalef(GLfloat x, GLfloat y, GLfloat z)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglScalef : public GemBase
{
    CPPEXTERN_HEADER(GEMglScalef, GemBase)

    public:

        //////////
        // Constructor
        GEMglScalef (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglScalef();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLfloat x;		// VAR
	virtual void	xMess 	(t_float);		// FUN glScalef GLfloat

	GLfloat y;		// VAR
	virtual void	yMess 	(t_float);		// FUN glScalef GLfloat

	GLfloat z;		// VAR
	virtual void	zMess 	(t_float);		// FUN glScalef GLfloat


    private:

        //////////
        // Static member functions

	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glScalef
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glScalef
	static void	zMessCallback	(void*, t_floatarg);		// CALLBACK glScalef

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
