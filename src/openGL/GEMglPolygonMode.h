/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glPolygonMode(GLenum face, GLenum mode)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLPOLYGONMODE_H_
#define INCLUDE_GEM_GLPOLYGONMODE_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglPolygonMode

    A Wrapper for the openGL-command "glPolygonMode(GLenum face, GLenum mode)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglPolygonMode : public GemBase
{
    CPPEXTERN_HEADER(GEMglPolygonMode, GemBase)

    public:

        //////////
        // Constructor
        GEMglPolygonMode (t_symbol*, t_symbol*);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglPolygonMode();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum face;		// VAR
	virtual void	faceMess 	(int);		// FUN glPolygonMode GLenum

	GLenum mode;		// VAR
	virtual void	modeMess 	(int);		// FUN glPolygonMode GLenum


    private:

        //////////
        // Static member functions

	static void	faceMessCallback	(void*, t_symbol*);		// CALLBACK glPolygonMode
	static void	modeMessCallback	(void*, t_symbol*);		// CALLBACK glPolygonMode

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
