/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glPixelTransferf(GLenum pname, GLfloat param)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLPIXELTRANSFERF_H_
#define INCLUDE_GEM_GLPIXELTRANSFERF_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglPixelTransferf

    A Wrapper for the openGL-command "glPixelTransferf(GLenum pname, GLfloat param)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglPixelTransferf : public GemBase
{
    CPPEXTERN_HEADER(GEMglPixelTransferf, GemBase)

    public:

        //////////
        // Constructor
        GEMglPixelTransferf (t_symbol*, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglPixelTransferf();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum pname;		// VAR
	virtual void	pnameMess 	(int);		// FUN glPixelTransferf GLenum

	GLfloat param;		// VAR
	virtual void	paramMess 	(t_float);		// FUN glPixelTransferf GLfloat


    private:

        //////////
        // Static member functions

	static void	pnameMessCallback	(void*, t_symbol*);		// CALLBACK glPixelTransferf
	static void	paramMessCallback	(void*, t_floatarg);		// CALLBACK glPixelTransferf

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
