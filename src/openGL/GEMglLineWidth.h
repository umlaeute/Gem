/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glLineWidth(GLfloat width)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLLINEWIDTH_H_
#define INCLUDE_GEM_GLLINEWIDTH_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglLineWidth

    A Wrapper for the openGL-command "glLineWidth(GLfloat width)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglLineWidth : public GemBase
{
    CPPEXTERN_HEADER(GEMglLineWidth, GemBase)

    public:

        //////////
        // Constructor
        GEMglLineWidth (t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglLineWidth();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLfloat width;		// VAR
	virtual void	widthMess 	(t_float);		// FUN glLineWidth GLfloat


    private:

        //////////
        // Static member functions

	static void	widthMessCallback	(void*, t_floatarg);		// CALLBACK glLineWidth

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
