/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexCoord4d(GLdouble s, GLdouble t, GLdouble r, GLdouble q)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXCOORD4D_H_
#define INCLUDE_GEM_GLTEXCOORD4D_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexCoord4d

    A Wrapper for the openGL-command "glTexCoord4d(GLdouble s, GLdouble t, GLdouble r, GLdouble q)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexCoord4d : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexCoord4d, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexCoord4d (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexCoord4d();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLdouble s;		// VAR
	virtual void	sMess 	(double);		// FUN glTexCoord4d GLdouble

	GLdouble t;		// VAR
	virtual void	tMess 	(double);		// FUN glTexCoord4d GLdouble

	GLdouble r;		// VAR
	virtual void	rMess 	(double);		// FUN glTexCoord4d GLdouble

	GLdouble q;		// VAR
	virtual void	qMess 	(double);		// FUN glTexCoord4d GLdouble


    private:

        //////////
        // Static member functions

	static void	sMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord4d
	static void	tMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord4d
	static void	rMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord4d
	static void	qMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord4d

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
