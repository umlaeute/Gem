/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLORTHO_H_
#define INCLUDE_GEM_GLORTHO_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglOrtho

    A Wrapper for the openGL-command "glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglOrtho : public GemBase
{
    CPPEXTERN_HEADER(GEMglOrtho, GemBase)

    public:

    //////////
    // Constructor
    GEMglOrtho (int argc, t_atom* argv);

    protected:

        //////////
        // Destructor
        virtual ~GEMglOrtho();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLdouble left;		// VAR
	virtual void	leftMess 	(double);		// FUN glOrtho GLdouble

	GLdouble right;		// VAR
	virtual void	rightMess 	(double);		// FUN glOrtho GLdouble

	GLdouble bottom;		// VAR
	virtual void	bottomMess 	(double);		// FUN glOrtho GLdouble

	GLdouble top;		// VAR
	virtual void	topMess 	(double);		// FUN glOrtho GLdouble

	GLdouble zNear;		// VAR
	virtual void	zNearMess 	(double);		// FUN glOrtho GLdouble

	GLdouble zFar;		// VAR
	virtual void	zFarMess 	(double);		// FUN glOrtho GLdouble


    private:

        //////////
        // Static member functions

	static void	leftMessCallback	(void*, t_floatarg);		// CALLBACK glOrtho
	static void	rightMessCallback	(void*, t_floatarg);		// CALLBACK glOrtho
	static void	bottomMessCallback	(void*, t_floatarg);		// CALLBACK glOrtho
	static void	topMessCallback	(void*, t_floatarg);		// CALLBACK glOrtho
	static void	zNearMessCallback	(void*, t_floatarg);		// CALLBACK glOrtho
	static void	zFarMessCallback	(void*, t_floatarg);		// CALLBACK glOrtho

	// we need some inlets
	t_inlet	*m_inlet[ 6 ];
};

#endif  // for header file
