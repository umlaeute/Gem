/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexCoord3f(GLfloat s, GLfloat t, GLfloat r)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXCOORD3F_H_
#define INCLUDE_GEM_GLTEXCOORD3F_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexCoord3f

    A Wrapper for the openGL-command "glTexCoord3f(GLfloat s, GLfloat t, GLfloat r)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexCoord3f : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexCoord3f, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexCoord3f (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexCoord3f();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLfloat s;		// VAR
	virtual void	sMess 	(t_float);		// FUN glTexCoord3f GLfloat

	GLfloat t;		// VAR
	virtual void	tMess 	(t_float);		// FUN glTexCoord3f GLfloat

	GLfloat r;		// VAR
	virtual void	rMess 	(t_float);		// FUN glTexCoord3f GLfloat


    private:

        //////////
        // Static member functions

	static void	sMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord3f
	static void	tMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord3f
	static void	rMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord3f

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
