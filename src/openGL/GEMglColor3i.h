/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glColor3i(GLint red, GLint green, GLint blue)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCOLOR3I_H_
#define INCLUDE_GEM_GLCOLOR3I_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglColor3i

    A Wrapper for the openGL-command "glColor3i(GLint red, GLint green, GLint blue)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglColor3i : public GemBase
{
    CPPEXTERN_HEADER(GEMglColor3i, GemBase)

    public:

        //////////
        // Constructor
        GEMglColor3i (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglColor3i();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLint red;		// VAR
	virtual void	redMess 	(int);		// FUN glColor3i GLint

	GLint green;		// VAR
	virtual void	greenMess 	(int);		// FUN glColor3i GLint

	GLint blue;		// VAR
	virtual void	blueMess 	(int);		// FUN glColor3i GLint


    private:

        //////////
        // Static member functions

	static void	redMessCallback	(void*, t_floatarg);		// CALLBACK glColor3i
	static void	greenMessCallback	(void*, t_floatarg);		// CALLBACK glColor3i
	static void	blueMessCallback	(void*, t_floatarg);		// CALLBACK glColor3i

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
