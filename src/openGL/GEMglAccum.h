/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glAccum(GLenum op, GLfloat value)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLACCUM_H_
#define INCLUDE_GEM_GLACCUM_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglAccum

    A Wrapper for the openGL-command "glAccum(GLenum op, GLfloat value)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglAccum : public GemBase
{
    CPPEXTERN_HEADER(GEMglAccum, GemBase)

    public:

        //////////
        // Constructor
        GEMglAccum (int, t_atom*);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglAccum();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum op;		// VAR
	virtual void	opMess 	(int, t_atom*);		// FUN glAccum GLenum

	GLfloat value;		// VAR
	virtual void	valueMess 	(t_float);		// FUN glAccum GLfloat


    private:

        //////////
        // Static member functions

	static void	opMessCallback	(void*, t_symbol*, int, t_atom*);		// CALLBACK glAccum
	static void	valueMessCallback	(void*, t_floatarg);		// CALLBACK glAccum

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
