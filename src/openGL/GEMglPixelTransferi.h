/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glPixelTransferi(GLenum pname, GLint param)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLPIXELTRANSFERI_H_
#define INCLUDE_GEM_GLPIXELTRANSFERI_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglPixelTransferi

    A Wrapper for the openGL-command "glPixelTransferi(GLenum pname, GLint param)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglPixelTransferi : public GemBase
{
    CPPEXTERN_HEADER(GEMglPixelTransferi, GemBase)

    public:

        //////////
        // Constructor
        GEMglPixelTransferi (t_symbol*, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglPixelTransferi();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum pname;		// VAR
	virtual void	pnameMess 	(int);		// FUN glPixelTransferi GLenum

	GLint param;		// VAR
	virtual void	paramMess 	(int);		// FUN glPixelTransferi GLint


    private:

        //////////
        // Static member functions

	static void	pnameMessCallback	(void*, t_symbol*);		// CALLBACK glPixelTransferi
	static void	paramMessCallback	(void*, t_floatarg);		// CALLBACK glPixelTransferi

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
