/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexCoord3d(GLdouble s, GLdouble t, GLdouble r)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXCOORD3D_H_
#define INCLUDE_GEM_GLTEXCOORD3D_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexCoord3d

    A Wrapper for the openGL-command "glTexCoord3d(GLdouble s, GLdouble t, GLdouble r)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexCoord3d : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexCoord3d, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexCoord3d (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexCoord3d();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLdouble s;		// VAR
	virtual void	sMess 	(double);		// FUN glTexCoord3d GLdouble

	GLdouble t;		// VAR
	virtual void	tMess 	(double);		// FUN glTexCoord3d GLdouble

	GLdouble r;		// VAR
	virtual void	rMess 	(double);		// FUN glTexCoord3d GLdouble


    private:

        //////////
        // Static member functions

	static void	sMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord3d
	static void	tMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord3d
	static void	rMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord3d

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
