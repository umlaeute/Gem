/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glRectf(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLRECTF_H_
#define INCLUDE_GEM_GLRECTF_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglRectf

    A Wrapper for the openGL-command "glRectf(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglRectf : public GemBase
{
    CPPEXTERN_HEADER(GEMglRectf, GemBase)

    public:

        //////////
        // Constructor
        GEMglRectf (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglRectf();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLfloat x1;		// VAR
	virtual void	x1Mess 	(t_float);		// FUN glRectf GLfloat

	GLfloat y1;		// VAR
	virtual void	y1Mess 	(t_float);		// FUN glRectf GLfloat

	GLfloat x2;		// VAR
	virtual void	x2Mess 	(t_float);		// FUN glRectf GLfloat

	GLfloat y2;		// VAR
	virtual void	y2Mess 	(t_float);		// FUN glRectf GLfloat


    private:

        //////////
        // Static member functions

	static void	x1MessCallback	(void*, t_floatarg);		// CALLBACK glRectf
	static void	y1MessCallback	(void*, t_floatarg);		// CALLBACK glRectf
	static void	x2MessCallback	(void*, t_floatarg);		// CALLBACK glRectf
	static void	y2MessCallback	(void*, t_floatarg);		// CALLBACK glRectf

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
