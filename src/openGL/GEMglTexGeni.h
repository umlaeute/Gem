/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexGeni(GLenum coord, GLenum pname, GLint param)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXGENI_H_
#define INCLUDE_GEM_GLTEXGENI_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexGeni

    A Wrapper for the openGL-command "glTexGeni(GLenum coord, GLenum pname, GLint param)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexGeni : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexGeni, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexGeni (t_symbol*, t_symbol*, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexGeni();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum coord;		// VAR
	virtual void	coordMess 	(int);		// FUN glTexGeni GLenum

	GLenum pname;		// VAR
	virtual void	pnameMess 	(int);		// FUN glTexGeni GLenum

	GLint param;		// VAR
	virtual void	paramMess 	(int);		// FUN glTexGeni GLint


    private:

        //////////
        // Static member functions

	static void	coordMessCallback	(void*, t_symbol*);		// CALLBACK glTexGeni
	static void	pnameMessCallback	(void*, t_symbol*);		// CALLBACK glTexGeni
	static void	paramMessCallback	(void*, t_floatarg);		// CALLBACK glTexGeni

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
