/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glScissor(GLint x, GLint y, GLsizei width, GLsizei height)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLSCISSOR_H_
#define INCLUDE_GEM_GLSCISSOR_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglScissor

    A Wrapper for the openGL-command "glScissor(GLint x, GLint y, GLsizei width, GLsizei height)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglScissor : public GemBase
{
    CPPEXTERN_HEADER(GEMglScissor, GemBase)

    public:

        //////////
        // Constructor
        GEMglScissor (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglScissor();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLint x;		// VAR
	virtual void	xMess 	(int);		// FUN glScissor GLint

	GLint y;		// VAR
	virtual void	yMess 	(int);		// FUN glScissor GLint

	GLsizei width;		// VAR
	virtual void	widthMess 	(int);		// FUN glScissor GLsizei

	GLsizei height;		// VAR
	virtual void	heightMess 	(int);		// FUN glScissor GLsizei


    private:

        //////////
        // Static member functions

	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glScissor
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glScissor
	static void	widthMessCallback	(void*, t_floatarg);		// CALLBACK glScissor
	static void	heightMessCallback	(void*, t_floatarg);		// CALLBACK glScissor

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
