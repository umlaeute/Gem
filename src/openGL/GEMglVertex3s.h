/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glVertex3s(GLshort x, GLshort y, GLshort z)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLVERTEX3S_H_
#define INCLUDE_GEM_GLVERTEX3S_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglVertex3s

    A Wrapper for the openGL-command "glVertex3s(GLshort x, GLshort y, GLshort z)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglVertex3s : public GemBase
{
    CPPEXTERN_HEADER(GEMglVertex3s, GemBase)

    public:

        //////////
        // Constructor
        GEMglVertex3s (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglVertex3s();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLshort x;		// VAR
	virtual void	xMess 	(int);		// FUN glVertex3s GLshort

	GLshort y;		// VAR
	virtual void	yMess 	(int);		// FUN glVertex3s GLshort

	GLshort z;		// VAR
	virtual void	zMess 	(int);		// FUN glVertex3s GLshort


    private:

        //////////
        // Static member functions

	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glVertex3s
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glVertex3s
	static void	zMessCallback	(void*, t_floatarg);		// CALLBACK glVertex3s

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
