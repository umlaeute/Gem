/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glNormal3s(GLshort nx, GLshort ny, GLshort nz)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLNORMAL3S_H_
#define INCLUDE_GEM_GLNORMAL3S_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglNormal3s

    A Wrapper for the openGL-command "glNormal3s(GLshort nx, GLshort ny, GLshort nz)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglNormal3s : public GemBase
{
    CPPEXTERN_HEADER(GEMglNormal3s, GemBase)

    public:

        //////////
        // Constructor
        GEMglNormal3s (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglNormal3s();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLshort nx;		// VAR
	virtual void	nxMess 	(int);		// FUN glNormal3s GLshort

	GLshort ny;		// VAR
	virtual void	nyMess 	(int);		// FUN glNormal3s GLshort

	GLshort nz;		// VAR
	virtual void	nzMess 	(int);		// FUN glNormal3s GLshort


    private:

        //////////
        // Static member functions

	static void	nxMessCallback	(void*, t_floatarg);		// CALLBACK glNormal3s
	static void	nyMessCallback	(void*, t_floatarg);		// CALLBACK glNormal3s
	static void	nzMessCallback	(void*, t_floatarg);		// CALLBACK glNormal3s

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
