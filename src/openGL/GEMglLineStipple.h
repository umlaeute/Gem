/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glLineStipple(GLint factor, GLushort pattern)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLLINESTIPPLE_H_
#define INCLUDE_GEM_GLLINESTIPPLE_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglLineStipple

    A Wrapper for the openGL-command "glLineStipple(GLint factor, GLushort pattern)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglLineStipple : public GemBase
{
    CPPEXTERN_HEADER(GEMglLineStipple, GemBase)

    public:

        //////////
        // Constructor
        GEMglLineStipple (t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglLineStipple();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLint factor;		// VAR
	virtual void	factorMess 	(int);		// FUN glLineStipple GLint

	GLushort pattern;		// VAR
	virtual void	patternMess 	(int);		// FUN glLineStipple GLushort


    private:

        //////////
        // Static member functions

	static void	factorMessCallback	(void*, t_floatarg);		// CALLBACK glLineStipple
	static void	patternMessCallback	(void*, t_floatarg);		// CALLBACK glLineStipple

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
