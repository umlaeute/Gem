/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glVertex4fv(GLfloat* v)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLVERTEX4FV_H_
#define INCLUDE_GEM_GLVERTEX4FV_H_

#include "Base/GemBase.h"
#include "Base/GemGLUtil.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglVertex4fv

    A Wrapper for the openGL-command "glVertex4fv(GLfloat* v)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglVertex4fv : public GemBase
{
    CPPEXTERN_HEADER(GEMglVertex4fv, GemBase)

    public:

        //////////
        // Constructor
        GEMglVertex4fv (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglVertex4fv();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	GLfloat m_v[4];		// VAR
	virtual void	vMess 	(t_float, t_float, t_float, t_float);		// FUN glVertex4fv GLfloat*


    private:

        //////////
        // Static member functions

	static void	vMessCallback	(void*, t_floatarg, t_floatarg, t_floatarg, t_floatarg);		// CALLBACK glVertex4fv

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
