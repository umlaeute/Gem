/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glNormal3i(GLint nx, GLint ny, GLint nz)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLNORMAL3I_H_
#define INCLUDE_GEM_GLNORMAL3I_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglNormal3i

    A Wrapper for the openGL-command "glNormal3i(GLint nx, GLint ny, GLint nz)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglNormal3i : public GemBase
{
    CPPEXTERN_HEADER(GEMglNormal3i, GemBase)

    public:

        //////////
        // Constructor
        GEMglNormal3i (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglNormal3i();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLint nx;		// VAR
	virtual void	nxMess 	(int);		// FUN glNormal3i GLint

	GLint ny;		// VAR
	virtual void	nyMess 	(int);		// FUN glNormal3i GLint

	GLint nz;		// VAR
	virtual void	nzMess 	(int);		// FUN glNormal3i GLint


    private:

        //////////
        // Static member functions

	static void	nxMessCallback	(void*, t_floatarg);		// CALLBACK glNormal3i
	static void	nyMessCallback	(void*, t_floatarg);		// CALLBACK glNormal3i
	static void	nzMessCallback	(void*, t_floatarg);		// CALLBACK glNormal3i

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
