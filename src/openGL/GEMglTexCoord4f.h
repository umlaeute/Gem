/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXCOORD4F_H_
#define INCLUDE_GEM_GLTEXCOORD4F_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexCoord4f

    A Wrapper for the openGL-command "glTexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexCoord4f : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexCoord4f, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexCoord4f (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexCoord4f();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLfloat s;		// VAR
	virtual void	sMess 	(t_float);		// FUN glTexCoord4f GLfloat

	GLfloat t;		// VAR
	virtual void	tMess 	(t_float);		// FUN glTexCoord4f GLfloat

	GLfloat r;		// VAR
	virtual void	rMess 	(t_float);		// FUN glTexCoord4f GLfloat

	GLfloat q;		// VAR
	virtual void	qMess 	(t_float);		// FUN glTexCoord4f GLfloat


    private:

        //////////
        // Static member functions

	static void	sMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord4f
	static void	tMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord4f
	static void	rMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord4f
	static void	qMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord4f

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
