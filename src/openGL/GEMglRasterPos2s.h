/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glRasterPos2s(GLshort x, GLshort y)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLRASTERPOS2S_H_
#define INCLUDE_GEM_GLRASTERPOS2S_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglRasterPos2s

    A Wrapper for the openGL-command "glRasterPos2s(GLshort x, GLshort y)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglRasterPos2s : public GemBase
{
    CPPEXTERN_HEADER(GEMglRasterPos2s, GemBase)

    public:

        //////////
        // Constructor
        GEMglRasterPos2s (t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglRasterPos2s();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLshort x;		// VAR
	virtual void	xMess 	(int);		// FUN glRasterPos2s GLshort

	GLshort y;		// VAR
	virtual void	yMess 	(int);		// FUN glRasterPos2s GLshort


    private:

        //////////
        // Static member functions

	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glRasterPos2s
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glRasterPos2s

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
