/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLFRUSTUM_H_
#define INCLUDE_GEM_GLFRUSTUM_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglFrustum

    A Wrapper for the openGL-command "glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglFrustum : public GemBase
{
    CPPEXTERN_HEADER(GEMglFrustum, GemBase)

    public:

    //////////
    // Constructor
    GEMglFrustum (int argc, t_atom* argv);

    protected:

        //////////
        // Destructor
        virtual ~GEMglFrustum();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLdouble left;		// VAR
	virtual void	leftMess 	(double);		// FUN glFrustum GLdouble

	GLdouble right;		// VAR
	virtual void	rightMess 	(double);		// FUN glFrustum GLdouble

	GLdouble bottom;		// VAR
	virtual void	bottomMess 	(double);		// FUN glFrustum GLdouble

	GLdouble top;		// VAR
	virtual void	topMess 	(double);		// FUN glFrustum GLdouble

	GLdouble zNear;		// VAR
	virtual void	zNearMess 	(double);		// FUN glFrustum GLdouble

	GLdouble zFar;		// VAR
	virtual void	zFarMess 	(double);		// FUN glFrustum GLdouble


    private:

        //////////
        // Static member functions

	static void	leftMessCallback	(void*, t_floatarg);		// CALLBACK glFrustum
	static void	rightMessCallback	(void*, t_floatarg);		// CALLBACK glFrustum
	static void	bottomMessCallback	(void*, t_floatarg);		// CALLBACK glFrustum
	static void	topMessCallback	(void*, t_floatarg);		// CALLBACK glFrustum
	static void	zNearMessCallback	(void*, t_floatarg);		// CALLBACK glFrustum
	static void	zFarMessCallback	(void*, t_floatarg);		// CALLBACK glFrustum

	// we need some inlets
	t_inlet	*m_inlet[ 6 ];
};

#endif  // for header file
