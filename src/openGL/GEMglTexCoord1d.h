/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexCoord1d(GLdouble s)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXCOORD1D_H_
#define INCLUDE_GEM_GLTEXCOORD1D_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexCoord1d

    A Wrapper for the openGL-command "glTexCoord1d(GLdouble s)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexCoord1d : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexCoord1d, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexCoord1d (t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexCoord1d();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLdouble s;		// VAR
	virtual void	sMess 	(double);		// FUN glTexCoord1d GLdouble


    private:

        //////////
        // Static member functions

	static void	sMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord1d

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
