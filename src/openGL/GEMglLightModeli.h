/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glLightModeli(GLenum pname, GLint param)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLLIGHTMODELI_H_
#define INCLUDE_GEM_GLLIGHTMODELI_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglLightModeli

    A Wrapper for the openGL-command "glLightModeli(GLenum pname, GLint param)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglLightModeli : public GemBase
{
    CPPEXTERN_HEADER(GEMglLightModeli, GemBase)

    public:

        //////////
        // Constructor
        GEMglLightModeli (t_symbol*, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglLightModeli();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum pname;		// VAR
	virtual void	pnameMess 	(int);		// FUN glLightModeli GLenum

	GLint param;		// VAR
	virtual void	paramMess 	(int);		// FUN glLightModeli GLint


    private:

        //////////
        // Static member functions

	static void	pnameMessCallback	(void*, t_symbol*);		// CALLBACK glLightModeli
	static void	paramMessCallback	(void*, t_floatarg);		// CALLBACK glLightModeli

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
