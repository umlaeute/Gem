/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexCoord1s(GLshort s)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXCOORD1S_H_
#define INCLUDE_GEM_GLTEXCOORD1S_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexCoord1s

    A Wrapper for the openGL-command "glTexCoord1s(GLshort s)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexCoord1s : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexCoord1s, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexCoord1s (t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexCoord1s();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLshort s;		// VAR
	virtual void	sMess 	(int);		// FUN glTexCoord1s GLshort


    private:

        //////////
        // Static member functions

	static void	sMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord1s

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
