/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glPixelStorei(GLenum pname, GLint param)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLPIXELSTOREI_H_
#define INCLUDE_GEM_GLPIXELSTOREI_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglPixelStorei

    A Wrapper for the openGL-command "glPixelStorei(GLenum pname, GLint param)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglPixelStorei : public GemBase
{
    CPPEXTERN_HEADER(GEMglPixelStorei, GemBase)

    public:

        //////////
        // Constructor
        GEMglPixelStorei (t_symbol*, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglPixelStorei();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum pname;		// VAR
	virtual void	pnameMess 	(int);		// FUN glPixelStorei GLenum

	GLint param;		// VAR
	virtual void	paramMess 	(int);		// FUN glPixelStorei GLint


    private:

        //////////
        // Static member functions

	static void	pnameMessCallback	(void*, t_symbol*);		// CALLBACK glPixelStorei
	static void	paramMessCallback	(void*, t_floatarg);		// CALLBACK glPixelStorei

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
