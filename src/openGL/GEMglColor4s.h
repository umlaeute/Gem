/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glColor4s(GLshort red, GLshort green, GLshort blue, GLshort alpha)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCOLOR4S_H_
#define INCLUDE_GEM_GLCOLOR4S_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglColor4s

    A Wrapper for the openGL-command "glColor4s(GLshort red, GLshort green, GLshort blue, GLshort alpha)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglColor4s : public GemBase
{
    CPPEXTERN_HEADER(GEMglColor4s, GemBase)

    public:

        //////////
        // Constructor
        GEMglColor4s (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglColor4s();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLshort red;		// VAR
	virtual void	redMess 	(int);		// FUN glColor4s GLshort

	GLshort green;		// VAR
	virtual void	greenMess 	(int);		// FUN glColor4s GLshort

	GLshort blue;		// VAR
	virtual void	blueMess 	(int);		// FUN glColor4s GLshort

	GLshort alpha;		// VAR
	virtual void	alphaMess 	(int);		// FUN glColor4s GLshort


    private:

        //////////
        // Static member functions

	static void	redMessCallback	(void*, t_floatarg);		// CALLBACK glColor4s
	static void	greenMessCallback	(void*, t_floatarg);		// CALLBACK glColor4s
	static void	blueMessCallback	(void*, t_floatarg);		// CALLBACK glColor4s
	static void	alphaMessCallback	(void*, t_floatarg);		// CALLBACK glColor4s

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
