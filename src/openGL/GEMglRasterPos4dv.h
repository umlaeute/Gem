/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glRasterPos4dv (GLdouble* v)"

  Copyright  (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLRASTERPOS4DV_H_
#define INCLUDE_GEM_GLRASTERPOS4DV_H_

#include "Base/GemBase.h"
#include "Base/GemGLUtil.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglRasterPos4dv

    A Wrapper for the openGL-command "glRasterPos4dv (GLdouble* v)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglRasterPos4dv : public GemBase
{
    CPPEXTERN_HEADER (GEMglRasterPos4dv, GemBase)

    public:

        //////////
        // Constructor
        GEMglRasterPos4dv  (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
	virtual~GEMglRasterPos4dv ();

        //////////
        // Do the rendering
        virtual void    render  (GemState *state);

       //////////
       // define and set the variables

	 GLdouble m_v[4];		// VAR
	virtual void	vMess 	 (t_float, t_float, t_float, t_float);		// FUN glRasterPos4dv GLdouble*


    private:

        //////////
        // Static member functions

	static void	vMessCallback	 (void*, t_floatarg, t_floatarg, t_floatarg, t_floatarg);		// CALLBACK glRasterPos4dv

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
