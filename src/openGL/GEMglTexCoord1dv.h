/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexCoord1dv(GLdouble* v)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXCOORD1DV_H_
#define INCLUDE_GEM_GLTEXCOORD1DV_H_

#include "Base/GemBase.h"
#include "Base/GemGLUtil.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexCoord1dv

    A Wrapper for the openGL-command "glTexCoord1dv(GLdouble* v)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexCoord1dv : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexCoord1dv, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexCoord1dv (t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexCoord1dv();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLdouble m_v[1];		// VAR
	virtual void	vMess 	(t_float);		// FUN glTexCoord1dv GLdouble*


    private:

        //////////
        // Static member functions

	static void	vMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord1dv

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
