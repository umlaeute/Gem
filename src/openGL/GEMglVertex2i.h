/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glVertex2i(GLint x, GLint y)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLVERTEX2I_H_
#define INCLUDE_GEM_GLVERTEX2I_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglVertex2i

    A Wrapper for the openGL-command "glVertex2i(GLint x, GLint y)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglVertex2i : public GemBase
{
    CPPEXTERN_HEADER(GEMglVertex2i, GemBase)

    public:

        //////////
        // Constructor
        GEMglVertex2i (t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglVertex2i();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLint x;		// VAR
	virtual void	xMess 	(int);		// FUN glVertex2i GLint

	GLint y;		// VAR
	virtual void	yMess 	(int);		// FUN glVertex2i GLint


    private:

        //////////
        // Static member functions

	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glVertex2i
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glVertex2i

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
