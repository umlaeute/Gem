/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexCoord1fv(GLfloat* v)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXCOORD1FV_H_
#define INCLUDE_GEM_GLTEXCOORD1FV_H_

#include "Base/GemBase.h"
#include "Base/GemGLUtil.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexCoord1fv

    A Wrapper for the openGL-command "glTexCoord1fv(GLfloat* v)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexCoord1fv : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexCoord1fv, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexCoord1fv (t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexCoord1fv();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLfloat m_v[1];		// VAR
	virtual void	vMess 	(t_float);		// FUN glTexCoord1fv GLfloat*


    private:

        //////////
        // Static member functions

	static void	vMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord1fv

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
