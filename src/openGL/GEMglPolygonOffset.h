/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glPolygonOffset(GLfloat factor, GLfloat units)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLPOLYGONOFFSET_H_
#define INCLUDE_GEM_GLPOLYGONOFFSET_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglPolygonOffset

    A Wrapper for the openGL-command "glPolygonOffset(GLfloat factor, GLfloat units)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglPolygonOffset : public GemBase
{
    CPPEXTERN_HEADER(GEMglPolygonOffset, GemBase)

    public:

        //////////
        // Constructor
        GEMglPolygonOffset (t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglPolygonOffset();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLfloat factor;		// VAR
	virtual void	factorMess 	(t_float);		// FUN glPolygonOffset GLfloat

	GLfloat units;		// VAR
	virtual void	unitsMess 	(t_float);		// FUN glPolygonOffset GLfloat


    private:

        //////////
        // Static member functions

	static void	factorMessCallback	(void*, t_floatarg);		// CALLBACK glPolygonOffset
	static void	unitsMessCallback	(void*, t_floatarg);		// CALLBACK glPolygonOffset

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
