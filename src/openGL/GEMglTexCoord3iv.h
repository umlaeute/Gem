/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexCoord3iv (GLint* v)"

  Copyright  (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXCOORD3IV_H_
#define INCLUDE_GEM_GLTEXCOORD3IV_H_

#include "Base/GemBase.h"
#include "Base/GemGLUtil.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexCoord3iv

    A Wrapper for the openGL-command "glTexCoord3iv (GLint* v)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexCoord3iv : public GemBase
{
    CPPEXTERN_HEADER (GEMglTexCoord3iv, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexCoord3iv  (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
	virtual~GEMglTexCoord3iv();

        //////////
        // Do the rendering
        virtual void    render  (GemState *state);

       //////////
       // define and set the variables

	 GLint m_v[3];		// VAR
	virtual void	vMess 	 (t_float, t_float, t_float);		// FUN glTexCoord3iv GLint*


    private:

        //////////
        // Static member functions

	static void	vMessCallback	(void*, t_floatarg, t_floatarg, t_floatarg);		// CALLBACK glColor3bv

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
