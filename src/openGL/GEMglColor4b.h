/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glColor4b(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCOLOR4B_H_
#define INCLUDE_GEM_GLCOLOR4B_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglColor4b

    A Wrapper for the openGL-command "glColor4b(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglColor4b : public GemBase
{
    CPPEXTERN_HEADER(GEMglColor4b, GemBase)

    public:

        //////////
        // Constructor
        GEMglColor4b (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglColor4b();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLbyte red;		// VAR
	virtual void	redMess 	(int);		// FUN glColor4b GLbyte

	GLbyte green;		// VAR
	virtual void	greenMess 	(int);		// FUN glColor4b GLbyte

	GLbyte blue;		// VAR
	virtual void	blueMess 	(int);		// FUN glColor4b GLbyte

	GLbyte alpha;		// VAR
	virtual void	alphaMess 	(int);		// FUN glColor4b GLbyte


    private:

        //////////
        // Static member functions

	static void	redMessCallback	(void*, t_floatarg);		// CALLBACK glColor4b
	static void	greenMessCallback	(void*, t_floatarg);		// CALLBACK glColor4b
	static void	blueMessCallback	(void*, t_floatarg);		// CALLBACK glColor4b
	static void	alphaMessCallback	(void*, t_floatarg);		// CALLBACK glColor4b

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
