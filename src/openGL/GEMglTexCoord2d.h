/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexCoord2d(GLdouble s, GLdouble t)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXCOORD2D_H_
#define INCLUDE_GEM_GLTEXCOORD2D_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexCoord2d

    A Wrapper for the openGL-command "glTexCoord2d(GLdouble s, GLdouble t)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexCoord2d : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexCoord2d, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexCoord2d (t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexCoord2d();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLdouble s;		// VAR
	virtual void	sMess 	(double);		// FUN glTexCoord2d GLdouble

	GLdouble t;		// VAR
	virtual void	tMess 	(double);		// FUN glTexCoord2d GLdouble


    private:

        //////////
        // Static member functions

	static void	sMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord2d
	static void	tMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord2d

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
