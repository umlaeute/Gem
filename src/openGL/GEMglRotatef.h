/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLROTATEF_H_
#define INCLUDE_GEM_GLROTATEF_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglRotatef

    A Wrapper for the openGL-command "glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglRotatef : public GemBase
{
    CPPEXTERN_HEADER(GEMglRotatef, GemBase)

    public:

        //////////
        // Constructor
        GEMglRotatef (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglRotatef();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLfloat angle;		// VAR
	virtual void	angleMess 	(t_float);		// FUN glRotatef GLfloat

	GLfloat x;		// VAR
	virtual void	xMess 	(t_float);		// FUN glRotatef GLfloat

	GLfloat y;		// VAR
	virtual void	yMess 	(t_float);		// FUN glRotatef GLfloat

	GLfloat z;		// VAR
	virtual void	zMess 	(t_float);		// FUN glRotatef GLfloat


    private:

        //////////
        // Static member functions

	static void	angleMessCallback	(void*, t_floatarg);		// CALLBACK glRotatef
	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glRotatef
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glRotatef
	static void	zMessCallback	(void*, t_floatarg);		// CALLBACK glRotatef

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
