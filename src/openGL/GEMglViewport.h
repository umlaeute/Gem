/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glViewport(GLint x, GLint y, GLsizei width, GLsizei height)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLVIEWPORT_H_
#define INCLUDE_GEM_GLVIEWPORT_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglViewport

    A Wrapper for the openGL-command "glViewport(GLint x, GLint y, GLsizei width, GLsizei height)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglViewport : public GemBase
{
    CPPEXTERN_HEADER(GEMglViewport, GemBase)

    public:

        //////////
        // Constructor
        GEMglViewport (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglViewport();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLint x;		// VAR
	virtual void	xMess 	(int);		// FUN glViewport GLint

	GLint y;		// VAR
	virtual void	yMess 	(int);		// FUN glViewport GLint

	GLsizei width;		// VAR
	virtual void	widthMess 	(int);		// FUN glViewport GLsizei

	GLsizei height;		// VAR
	virtual void	heightMess 	(int);		// FUN glViewport GLsizei


    private:

        //////////
        // Static member functions

	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glViewport
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glViewport
	static void	widthMessCallback	(void*, t_floatarg);		// CALLBACK glViewport
	static void	heightMessCallback	(void*, t_floatarg);		// CALLBACK glViewport

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
