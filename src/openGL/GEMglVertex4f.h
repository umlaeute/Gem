/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLVERTEX4F_H_
#define INCLUDE_GEM_GLVERTEX4F_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglVertex4f

    A Wrapper for the openGL-command "glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglVertex4f : public GemBase
{
    CPPEXTERN_HEADER(GEMglVertex4f, GemBase)

    public:

        //////////
        // Constructor
        GEMglVertex4f (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglVertex4f();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLfloat x;		// VAR
	virtual void	xMess 	(t_float);		// FUN glVertex4f GLfloat

	GLfloat y;		// VAR
	virtual void	yMess 	(t_float);		// FUN glVertex4f GLfloat

	GLfloat z;		// VAR
	virtual void	zMess 	(t_float);		// FUN glVertex4f GLfloat

	GLfloat w;		// VAR
	virtual void	wMess 	(t_float);		// FUN glVertex4f GLfloat


    private:

        //////////
        // Static member functions

	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glVertex4f
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glVertex4f
	static void	zMessCallback	(void*, t_floatarg);		// CALLBACK glVertex4f
	static void	wMessCallback	(void*, t_floatarg);		// CALLBACK glVertex4f

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
