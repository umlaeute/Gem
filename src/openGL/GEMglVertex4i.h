/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glVertex4i(GLint x, GLint y, GLint z, GLint w)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLVERTEX4I_H_
#define INCLUDE_GEM_GLVERTEX4I_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglVertex4i

    A Wrapper for the openGL-command "glVertex4i(GLint x, GLint y, GLint z, GLint w)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglVertex4i : public GemBase
{
    CPPEXTERN_HEADER(GEMglVertex4i, GemBase)

    public:

        //////////
        // Constructor
        GEMglVertex4i (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglVertex4i();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLint x;		// VAR
	virtual void	xMess 	(int);		// FUN glVertex4i GLint

	GLint y;		// VAR
	virtual void	yMess 	(int);		// FUN glVertex4i GLint

	GLint z;		// VAR
	virtual void	zMess 	(int);		// FUN glVertex4i GLint

	GLint w;		// VAR
	virtual void	wMess 	(int);		// FUN glVertex4i GLint


    private:

        //////////
        // Static member functions

	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glVertex4i
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glVertex4i
	static void	zMessCallback	(void*, t_floatarg);		// CALLBACK glVertex4i
	static void	wMessCallback	(void*, t_floatarg);		// CALLBACK glVertex4i

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
