/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexParameterf(GLenum target, GLenum pname, GLfloat param)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXPARAMETERF_H_
#define INCLUDE_GEM_GLTEXPARAMETERF_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexParameterf

    A Wrapper for the openGL-command "glTexParameterf(GLenum target, GLenum pname, GLfloat param)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexParameterf : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexParameterf, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexParameterf (t_symbol*, t_symbol*, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexParameterf();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum target;		// VAR
	virtual void	targetMess 	(int);		// FUN glTexParameterf GLenum

	GLenum pname;		// VAR
	virtual void	pnameMess 	(int);		// FUN glTexParameterf GLenum

	GLfloat param;		// VAR
	virtual void	paramMess 	(t_float);		// FUN glTexParameterf GLfloat


    private:

        //////////
        // Static member functions

	static void	targetMessCallback	(void*, t_symbol*);		// CALLBACK glTexParameterf
	static void	pnameMessCallback	(void*, t_symbol*);		// CALLBACK glTexParameterf
	static void	paramMessCallback	(void*, t_floatarg);		// CALLBACK glTexParameterf

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
