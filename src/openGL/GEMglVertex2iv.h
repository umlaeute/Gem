/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glVertex2iv(GLint* v)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLVERTEX2IV_H_
#define INCLUDE_GEM_GLVERTEX2IV_H_

#include "Base/GemBase.h"
#include "Base/GemGLUtil.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglVertex2iv

    A Wrapper for the openGL-command "glVertex2iv(GLint* v)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglVertex2iv : public GemBase
{
    CPPEXTERN_HEADER(GEMglVertex2iv, GemBase)

    public:

        //////////
        // Constructor
	GEMglVertex2iv (t_floatarg, t_floatarg);	//CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglVertex2iv();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	GLint m_v[2];
	virtual void vMess	(t_float, t_float);


    private:

        //////////
        // Static member functions

	static void vMessCallback	(void*, t_floatarg, t_floatarg);		// CALLBACK glVertex2sv

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
