/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glColor4i(GLint red, GLint green, GLint blue, GLint alpha)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCOLOR4I_H_
#define INCLUDE_GEM_GLCOLOR4I_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglColor4i

    A Wrapper for the openGL-command "glColor4i(GLint red, GLint green, GLint blue, GLint alpha)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglColor4i : public GemBase
{
    CPPEXTERN_HEADER(GEMglColor4i, GemBase)

    public:

        //////////
        // Constructor
        GEMglColor4i (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglColor4i();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLint red;		// VAR
	virtual void	redMess 	(int);		// FUN glColor4i GLint

	GLint green;		// VAR
	virtual void	greenMess 	(int);		// FUN glColor4i GLint

	GLint blue;		// VAR
	virtual void	blueMess 	(int);		// FUN glColor4i GLint

	GLint alpha;		// VAR
	virtual void	alphaMess 	(int);		// FUN glColor4i GLint


    private:

        //////////
        // Static member functions

	static void	redMessCallback	(void*, t_floatarg);		// CALLBACK glColor4i
	static void	greenMessCallback	(void*, t_floatarg);		// CALLBACK glColor4i
	static void	blueMessCallback	(void*, t_floatarg);		// CALLBACK glColor4i
	static void	alphaMessCallback	(void*, t_floatarg);		// CALLBACK glColor4i

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
