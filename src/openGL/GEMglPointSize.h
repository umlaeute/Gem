/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glPointSize(GLfloat size)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLPOINTSIZE_H_
#define INCLUDE_GEM_GLPOINTSIZE_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglPointSize

    A Wrapper for the openGL-command "glPointSize(GLfloat size)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglPointSize : public GemBase
{
    CPPEXTERN_HEADER(GEMglPointSize, GemBase)

    public:

        //////////
        // Constructor
        GEMglPointSize (t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglPointSize();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLfloat size;		// VAR
	virtual void	sizeMess 	(t_float);		// FUN glPointSize GLfloat


    private:

        //////////
        // Static member functions

	static void	sizeMessCallback	(void*, t_floatarg);		// CALLBACK glPointSize

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
