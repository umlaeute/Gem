/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glRasterPos2fv (GLfloat* v)"

  Copyright  (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLRASTERPOS2FV_H_
#define INCLUDE_GEM_GLRASTERPOS2FV_H_

#include "Base/GemBase.h"
#include "Base/GemGLUtil.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglRasterPos2fv

    A Wrapper for the openGL-command "glRasterPos2fv (GLfloat* v)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglRasterPos2fv : public GemBase
{
    CPPEXTERN_HEADER (GEMglRasterPos2fv, GemBase)

    public:

        //////////
        // Constructor
        GEMglRasterPos2fv  (t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
	virtual~GEMglRasterPos2fv ();

        //////////
        // Do the rendering
        virtual void    render  (GemState *state);

       //////////
       // define and set the variables

	 GLfloat m_v[2];		// VAR
	virtual void	vMess 	 (t_float, t_float);		// FUN glRasterPos2fv GLfloat*


    private:

        //////////
        // Static member functions

	static void	vMessCallback	 (void*, t_floatarg, t_floatarg);		// CALLBACK glRasterPos2fv

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
