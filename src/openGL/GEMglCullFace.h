/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glCullFace(GLenum mode)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCULLFACE_H_
#define INCLUDE_GEM_GLCULLFACE_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglCullFace

    A Wrapper for the openGL-command "glCullFace(GLenum mode)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglCullFace : public GemBase
{
    CPPEXTERN_HEADER(GEMglCullFace, GemBase)

    public:

        //////////
        // Constructor
        GEMglCullFace (t_symbol*);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglCullFace();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum mode;		// VAR
	virtual void	modeMess 	(int);		// FUN glCullFace GLenum


    private:

        //////////
        // Static member functions

	static void	modeMessCallback	(void*, t_symbol*);		// CALLBACK glCullFace

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
