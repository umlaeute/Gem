/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glNormal3b(GLbyte nx, GLbyte ny, GLbyte nz)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLNORMAL3B_H_
#define INCLUDE_GEM_GLNORMAL3B_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglNormal3b

    A Wrapper for the openGL-command "glNormal3b(GLbyte nx, GLbyte ny, GLbyte nz)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglNormal3b : public GemBase
{
    CPPEXTERN_HEADER(GEMglNormal3b, GemBase)

    public:

        //////////
        // Constructor
        GEMglNormal3b (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglNormal3b();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLbyte nx;		// VAR
	virtual void	nxMess 	(int);		// FUN glNormal3b GLbyte

	GLbyte ny;		// VAR
	virtual void	nyMess 	(int);		// FUN glNormal3b GLbyte

	GLbyte nz;		// VAR
	virtual void	nzMess 	(int);		// FUN glNormal3b GLbyte


    private:

        //////////
        // Static member functions

	static void	nxMessCallback	(void*, t_floatarg);		// CALLBACK glNormal3b
	static void	nyMessCallback	(void*, t_floatarg);		// CALLBACK glNormal3b
	static void	nzMessCallback	(void*, t_floatarg);		// CALLBACK glNormal3b

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
