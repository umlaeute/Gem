/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexCoord2i(GLint s, GLint t)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXCOORD2I_H_
#define INCLUDE_GEM_GLTEXCOORD2I_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexCoord2i

    A Wrapper for the openGL-command "glTexCoord2i(GLint s, GLint t)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexCoord2i : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexCoord2i, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexCoord2i (t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexCoord2i();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLint s;		// VAR
	virtual void	sMess 	(int);		// FUN glTexCoord2i GLint

	GLint t;		// VAR
	virtual void	tMess 	(int);		// FUN glTexCoord2i GLint


    private:

        //////////
        // Static member functions

	static void	sMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord2i
	static void	tMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord2i

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
