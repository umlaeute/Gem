/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glMateriali(GLenum face, GLenum pname, GLint param)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLMATERIALI_H_
#define INCLUDE_GEM_GLMATERIALI_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglMateriali

    A Wrapper for the openGL-command "glMateriali(GLenum face, GLenum pname, GLint param)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglMateriali : public GemBase
{
    CPPEXTERN_HEADER(GEMglMateriali, GemBase)

    public:

        //////////
        // Constructor
        GEMglMateriali (t_symbol*, t_symbol*, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglMateriali();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum face;		// VAR
	virtual void	faceMess 	(int);		// FUN glMateriali GLenum

	GLenum pname;		// VAR
	virtual void	pnameMess 	(int);		// FUN glMateriali GLenum

	GLint param;		// VAR
	virtual void	paramMess 	(int);		// FUN glMateriali GLint


    private:

        //////////
        // Static member functions

	static void	faceMessCallback	(void*, t_symbol*);		// CALLBACK glMateriali
	static void	pnameMessCallback	(void*, t_symbol*);		// CALLBACK glMateriali
	static void	paramMessCallback	(void*, t_floatarg);		// CALLBACK glMateriali

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
