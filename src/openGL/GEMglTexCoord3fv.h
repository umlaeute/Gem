/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexCoord3fv (GLfloat* v)"

  Copyright  (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXCOORD3FV_H_
#define INCLUDE_GEM_GLTEXCOORD3FV_H_

#include "Base/GemBase.h"
#include "Base/GemGLUtil.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexCoord3fv

    A Wrapper for the openGL-command "glTexCoord3fv (GLfloat* v)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexCoord3fv : public GemBase
{
    CPPEXTERN_HEADER (GEMglTexCoord3fv, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexCoord3fv  (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
	virtual~GEMglTexCoord3fv();

        //////////
        // Do the rendering
        virtual void    render  (GemState *state);

       //////////
       // define and set the variables

	 GLfloat m_v[3];		// VAR
	virtual void	vMess 	 (t_float, t_float, t_float);		// FUN glTexCoord3fv GLfloat*


    private:

        //////////
        // Static member functions

	static void	vMessCallback	(void*, t_floatarg, t_floatarg, t_floatarg);		// CALLBACK glColor3bv

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
