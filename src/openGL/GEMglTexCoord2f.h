/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexCoord2f(GLfloat s, GLfloat t)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXCOORD2F_H_
#define INCLUDE_GEM_GLTEXCOORD2F_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexCoord2f

    A Wrapper for the openGL-command "glTexCoord2f(GLfloat s, GLfloat t)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexCoord2f : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexCoord2f, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexCoord2f (t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexCoord2f();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLfloat s;		// VAR
	virtual void	sMess 	(t_float);		// FUN glTexCoord2f GLfloat

	GLfloat t;		// VAR
	virtual void	tMess 	(t_float);		// FUN glTexCoord2f GLfloat


    private:

        //////////
        // Static member functions

	static void	sMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord2f
	static void	tMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord2f

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
