/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glColor3b(GLbyte red, GLbyte green, GLbyte blue)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCOLOR3B_H_
#define INCLUDE_GEM_GLCOLOR3B_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglColor3b

    A Wrapper for the openGL-command "glColor3b(GLbyte red, GLbyte green, GLbyte blue)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglColor3b : public GemBase
{
    CPPEXTERN_HEADER(GEMglColor3b, GemBase)

    public:

        //////////
        // Constructor
        GEMglColor3b (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglColor3b();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLbyte red;		// VAR
	virtual void	redMess 	(int);		// FUN glColor3b GLbyte

	GLbyte green;		// VAR
	virtual void	greenMess 	(int);		// FUN glColor3b GLbyte

	GLbyte blue;		// VAR
	virtual void	blueMess 	(int);		// FUN glColor3b GLbyte


    private:

        //////////
        // Static member functions

	static void	redMessCallback	(void*, t_floatarg);		// CALLBACK glColor3b
	static void	greenMessCallback	(void*, t_floatarg);		// CALLBACK glColor3b
	static void	blueMessCallback	(void*, t_floatarg);		// CALLBACK glColor3b

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
