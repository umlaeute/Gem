/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glClearAccum(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCLEARACCUM_H_
#define INCLUDE_GEM_GLCLEARACCUM_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglClearAccum

    A Wrapper for the openGL-command "glClearAccum(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglClearAccum : public GemBase
{
    CPPEXTERN_HEADER(GEMglClearAccum, GemBase)

    public:

        //////////
        // Constructor
        GEMglClearAccum (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglClearAccum();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLfloat red;		// VAR
	virtual void	redMess 	(t_float);		// FUN glClearAccum GLfloat

	GLfloat green;		// VAR
	virtual void	greenMess 	(t_float);		// FUN glClearAccum GLfloat

	GLfloat blue;		// VAR
	virtual void	blueMess 	(t_float);		// FUN glClearAccum GLfloat

	GLfloat alpha;		// VAR
	virtual void	alphaMess 	(t_float);		// FUN glClearAccum GLfloat


    private:

        //////////
        // Static member functions

	static void	redMessCallback	(void*, t_floatarg);		// CALLBACK glClearAccum
	static void	greenMessCallback	(void*, t_floatarg);		// CALLBACK glClearAccum
	static void	blueMessCallback	(void*, t_floatarg);		// CALLBACK glClearAccum
	static void	alphaMessCallback	(void*, t_floatarg);		// CALLBACK glClearAccum

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
