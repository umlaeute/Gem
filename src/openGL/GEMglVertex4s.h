/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glVertex4s(GLshort x, GLshort y, GLshort z, GLshort w)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLVERTEX4S_H_
#define INCLUDE_GEM_GLVERTEX4S_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglVertex4s

    A Wrapper for the openGL-command "glVertex4s(GLshort x, GLshort y, GLshort z, GLshort w)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglVertex4s : public GemBase
{
    CPPEXTERN_HEADER(GEMglVertex4s, GemBase)

    public:

        //////////
        // Constructor
        GEMglVertex4s (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglVertex4s();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLshort x;		// VAR
	virtual void	xMess 	(int);		// FUN glVertex4s GLshort

	GLshort y;		// VAR
	virtual void	yMess 	(int);		// FUN glVertex4s GLshort

	GLshort z;		// VAR
	virtual void	zMess 	(int);		// FUN glVertex4s GLshort

	GLshort w;		// VAR
	virtual void	wMess 	(int);		// FUN glVertex4s GLshort


    private:

        //////////
        // Static member functions

	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glVertex4s
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glVertex4s
	static void	zMessCallback	(void*, t_floatarg);		// CALLBACK glVertex4s
	static void	wMessCallback	(void*, t_floatarg);		// CALLBACK glVertex4s

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
