/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexCoord2s(GLshort s, GLshort t)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXCOORD2S_H_
#define INCLUDE_GEM_GLTEXCOORD2S_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexCoord2s

    A Wrapper for the openGL-command "glTexCoord2s(GLshort s, GLshort t)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexCoord2s : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexCoord2s, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexCoord2s (t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexCoord2s();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLshort s;		// VAR
	virtual void	sMess 	(int);		// FUN glTexCoord2s GLshort

	GLshort t;		// VAR
	virtual void	tMess 	(int);		// FUN glTexCoord2s GLshort


    private:

        //////////
        // Static member functions

	static void	sMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord2s
	static void	tMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord2s

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
