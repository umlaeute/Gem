/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexCoord3i(GLint s, GLint t, GLint r)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXCOORD3I_H_
#define INCLUDE_GEM_GLTEXCOORD3I_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexCoord3i

    A Wrapper for the openGL-command "glTexCoord3i(GLint s, GLint t, GLint r)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexCoord3i : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexCoord3i, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexCoord3i (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexCoord3i();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLint s;		// VAR
	virtual void	sMess 	(int);		// FUN glTexCoord3i GLint

	GLint t;		// VAR
	virtual void	tMess 	(int);		// FUN glTexCoord3i GLint

	GLint r;		// VAR
	virtual void	rMess 	(int);		// FUN glTexCoord3i GLint


    private:

        //////////
        // Static member functions

	static void	sMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord3i
	static void	tMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord3i
	static void	rMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord3i

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
