/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexEnvi(GLenum target, GLenum pname, GLint param)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXENVI_H_
#define INCLUDE_GEM_GLTEXENVI_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexEnvi

    A Wrapper for the openGL-command "glTexEnvi(GLenum target, GLenum pname, GLint param)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexEnvi : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexEnvi, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexEnvi (t_symbol*, t_symbol*, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexEnvi();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum target;		// VAR
	virtual void	targetMess 	(int);		// FUN glTexEnvi GLenum

	GLenum pname;		// VAR
	virtual void	pnameMess 	(int);		// FUN glTexEnvi GLenum

	GLint param;		// VAR
	virtual void	paramMess 	(int);		// FUN glTexEnvi GLint


    private:

        //////////
        // Static member functions

	static void	targetMessCallback	(void*, t_symbol*);		// CALLBACK glTexEnvi
	static void	pnameMessCallback	(void*, t_symbol*);		// CALLBACK glTexEnvi
	static void	paramMessCallback	(void*, t_floatarg);		// CALLBACK glTexEnvi

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
