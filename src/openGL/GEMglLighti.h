/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glLighti(GLenum light, GLenum pname, GLint param)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLLIGHTI_H_
#define INCLUDE_GEM_GLLIGHTI_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglLighti

    A Wrapper for the openGL-command "glLighti(GLenum light, GLenum pname, GLint param)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglLighti : public GemBase
{
    CPPEXTERN_HEADER(GEMglLighti, GemBase)

    public:

        //////////
        // Constructor
        GEMglLighti (t_symbol*, t_symbol*, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglLighti();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum light;		// VAR
	virtual void	lightMess 	(int);		// FUN glLighti GLenum

	GLenum pname;		// VAR
	virtual void	pnameMess 	(int);		// FUN glLighti GLenum

	GLint param;		// VAR
	virtual void	paramMess 	(int);		// FUN glLighti GLint


    private:

        //////////
        // Static member functions

	static void	lightMessCallback	(void*, t_symbol*);		// CALLBACK glLighti
	static void	pnameMessCallback	(void*, t_symbol*);		// CALLBACK glLighti
	static void	paramMessCallback	(void*, t_floatarg);		// CALLBACK glLighti

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
