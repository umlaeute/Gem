/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glLightModelf(GLenum pname, GLfloat param)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLLIGHTMODELF_H_
#define INCLUDE_GEM_GLLIGHTMODELF_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglLightModelf

    A Wrapper for the openGL-command "glLightModelf(GLenum pname, GLfloat param)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglLightModelf : public GemBase
{
    CPPEXTERN_HEADER(GEMglLightModelf, GemBase)

    public:

        //////////
        // Constructor
        GEMglLightModelf (t_symbol*, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglLightModelf();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum pname;		// VAR
	virtual void	pnameMess 	(int);		// FUN glLightModelf GLenum

	GLfloat param;		// VAR
	virtual void	paramMess 	(t_float);		// FUN glLightModelf GLfloat


    private:

        //////////
        // Static member functions

	static void	pnameMessCallback	(void*, t_symbol*);		// CALLBACK glLightModelf
	static void	paramMessCallback	(void*, t_floatarg);		// CALLBACK glLightModelf

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
