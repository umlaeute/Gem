/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glColor3s(GLshort red, GLshort green, GLshort blue)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCOLOR3S_H_
#define INCLUDE_GEM_GLCOLOR3S_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglColor3s

    A Wrapper for the openGL-command "glColor3s(GLshort red, GLshort green, GLshort blue)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglColor3s : public GemBase
{
    CPPEXTERN_HEADER(GEMglColor3s, GemBase)

    public:

        //////////
        // Constructor
        GEMglColor3s (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglColor3s();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLshort red;		// VAR
	virtual void	redMess 	(int);		// FUN glColor3s GLshort

	GLshort green;		// VAR
	virtual void	greenMess 	(int);		// FUN glColor3s GLshort

	GLshort blue;		// VAR
	virtual void	blueMess 	(int);		// FUN glColor3s GLshort


    private:

        //////////
        // Static member functions

	static void	redMessCallback	(void*, t_floatarg);		// CALLBACK glColor3s
	static void	greenMessCallback	(void*, t_floatarg);		// CALLBACK glColor3s
	static void	blueMessCallback	(void*, t_floatarg);		// CALLBACK glColor3s

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
