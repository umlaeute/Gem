/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexCoord1iv(GLint* v)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXCOORD1IV_H_
#define INCLUDE_GEM_GLTEXCOORD1IV_H_

#include "Base/GemBase.h"
#include "Base/GemGLUtil.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexCoord1iv

    A Wrapper for the openGL-command "glTexCoord1iv(GLint* v)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexCoord1iv : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexCoord1iv, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexCoord1iv (t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexCoord1iv();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLint m_v[1];		// VAR
	virtual void	vMess 	(t_float);		// FUN glTexCoord1iv GLint*


    private:

        //////////
        // Static member functions

	static void	vMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord1iv

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
