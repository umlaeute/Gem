/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexCoord2dv (GLdouble* v)"

  Copyright  (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXCOORD2DV_H_
#define INCLUDE_GEM_GLTEXCOORD2DV_H_

#include "Base/GemBase.h"
#include "Base/GemGLUtil.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexCoord2dv

    A Wrapper for the openGL-command "glTexCoord2dv (GLdouble* v)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexCoord2dv : public GemBase
{
    CPPEXTERN_HEADER (GEMglTexCoord2dv, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexCoord2dv  (t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
	virtual~GEMglTexCoord2dv ();

        //////////
        // Do the rendering
        virtual void    render  (GemState *state);

       //////////
       // define and set the variables

	 GLdouble m_v[2];		// VAR
	virtual void	vMess 	 (t_float, t_float);		// FUN glTexCoord2dv GLdouble*


    private:

        //////////
        // Static member functions

	static void	vMessCallback	 (void*, t_floatarg, t_floatarg);		// CALLBACK glTexCoord2dv

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
