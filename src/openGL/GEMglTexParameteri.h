/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexParameteri(GLenum target, GLenum pname, GLint param)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXPARAMETERI_H_
#define INCLUDE_GEM_GLTEXPARAMETERI_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexParameteri

    A Wrapper for the openGL-command "glTexParameteri(GLenum target, GLenum pname, GLint param)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexParameteri : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexParameteri, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexParameteri (t_symbol*, t_symbol*, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexParameteri();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum target;		// VAR
	virtual void	targetMess 	(int);		// FUN glTexParameteri GLenum

	GLenum pname;		// VAR
	virtual void	pnameMess 	(int);		// FUN glTexParameteri GLenum

	GLint param;		// VAR
	virtual void	paramMess 	(int);		// FUN glTexParameteri GLint


    private:

        //////////
        // Static member functions

	static void	targetMessCallback	(void*, t_symbol*);		// CALLBACK glTexParameteri
	static void	pnameMessCallback	(void*, t_symbol*);		// CALLBACK glTexParameteri
	static void	paramMessCallback	(void*, t_floatarg);		// CALLBACK glTexParameteri

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
