/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glRasterPos2i(GLint x, GLint y)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLRASTERPOS2I_H_
#define INCLUDE_GEM_GLRASTERPOS2I_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglRasterPos2i

    A Wrapper for the openGL-command "glRasterPos2i(GLint x, GLint y)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglRasterPos2i : public GemBase
{
    CPPEXTERN_HEADER(GEMglRasterPos2i, GemBase)

    public:

        //////////
        // Constructor
        GEMglRasterPos2i (t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglRasterPos2i();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLint x;		// VAR
	virtual void	xMess 	(int);		// FUN glRasterPos2i GLint

	GLint y;		// VAR
	virtual void	yMess 	(int);		// FUN glRasterPos2i GLint


    private:

        //////////
        // Static member functions

	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glRasterPos2i
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glRasterPos2i

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
