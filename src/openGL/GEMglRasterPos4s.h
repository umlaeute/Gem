/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glRasterPos4s(GLshort x, GLshort y, GLshort z, GLshort w)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLRASTERPOS4S_H_
#define INCLUDE_GEM_GLRASTERPOS4S_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglRasterPos4s

    A Wrapper for the openGL-command "glRasterPos4s(GLshort x, GLshort y, GLshort z, GLshort w)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglRasterPos4s : public GemBase
{
    CPPEXTERN_HEADER(GEMglRasterPos4s, GemBase)

    public:

        //////////
        // Constructor
        GEMglRasterPos4s (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglRasterPos4s();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLshort x;		// VAR
	virtual void	xMess 	(int);		// FUN glRasterPos4s GLshort

	GLshort y;		// VAR
	virtual void	yMess 	(int);		// FUN glRasterPos4s GLshort

	GLshort z;		// VAR
	virtual void	zMess 	(int);		// FUN glRasterPos4s GLshort

	GLshort w;		// VAR
	virtual void	wMess 	(int);		// FUN glRasterPos4s GLshort


    private:

        //////////
        // Static member functions

	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glRasterPos4s
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glRasterPos4s
	static void	zMessCallback	(void*, t_floatarg);		// CALLBACK glRasterPos4s
	static void	wMessCallback	(void*, t_floatarg);		// CALLBACK glRasterPos4s

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
