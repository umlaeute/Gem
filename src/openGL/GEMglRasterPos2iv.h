/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glRasterPos2iv (GLint* v)"

  Copyright  (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLRASTERPOS2IV_H_
#define INCLUDE_GEM_GLRASTERPOS2IV_H_

#include "Base/GemBase.h"
#include "Base/GemGLUtil.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglRasterPos2iv

    A Wrapper for the openGL-command "glRasterPos2iv (GLint* v)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglRasterPos2iv : public GemBase
{
    CPPEXTERN_HEADER (GEMglRasterPos2iv, GemBase)

    public:

        //////////
        // Constructor
        GEMglRasterPos2iv  (t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
	virtual~GEMglRasterPos2iv ();

        //////////
        // Do the rendering
        virtual void    render  (GemState *state);

       //////////
       // define and set the variables

	 GLint m_v[2];		// VAR
	virtual void	vMess 	 (t_float, t_float);		// FUN glRasterPos2iv GLint*


    private:

        //////////
        // Static member functions

	static void	vMessCallback	 (void*, t_floatarg, t_floatarg);		// CALLBACK glRasterPos2iv

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
