/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glLightf(GLenum light, GLenum pname, GLfloat param)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLLIGHTF_H_
#define INCLUDE_GEM_GLLIGHTF_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglLightf

    A Wrapper for the openGL-command "glLightf(GLenum light, GLenum pname, GLfloat param)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglLightf : public GemBase
{
    CPPEXTERN_HEADER(GEMglLightf, GemBase)

    public:

        //////////
        // Constructor
        GEMglLightf (t_symbol*, t_symbol*, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglLightf();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum light;		// VAR
	virtual void	lightMess 	(int);		// FUN glLightf GLenum

	GLenum pname;		// VAR
	virtual void	pnameMess 	(int);		// FUN glLightf GLenum

	GLfloat param;		// VAR
	virtual void	paramMess 	(t_float);		// FUN glLightf GLfloat


    private:

        //////////
        // Static member functions

	static void	lightMessCallback	(void*, t_symbol*);		// CALLBACK glLightf
	static void	pnameMessCallback	(void*, t_symbol*);		// CALLBACK glLightf
	static void	paramMessCallback	(void*, t_floatarg);		// CALLBACK glLightf

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
