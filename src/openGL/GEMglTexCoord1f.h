/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexCoord1f(GLfloat s)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXCOORD1F_H_
#define INCLUDE_GEM_GLTEXCOORD1F_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexCoord1f

    A Wrapper for the openGL-command "glTexCoord1f(GLfloat s)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexCoord1f : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexCoord1f, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexCoord1f (t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexCoord1f();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLfloat s;		// VAR
	virtual void	sMess 	(t_float);		// FUN glTexCoord1f GLfloat


    private:

        //////////
        // Static member functions

	static void	sMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord1f

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
