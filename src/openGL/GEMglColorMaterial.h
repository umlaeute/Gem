/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glColorMaterial(GLenum face, GLenum mode)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCOLORMATERIAL_H_
#define INCLUDE_GEM_GLCOLORMATERIAL_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglColorMaterial

    A Wrapper for the openGL-command "glColorMaterial(GLenum face, GLenum mode)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglColorMaterial : public GemBase
{
    CPPEXTERN_HEADER(GEMglColorMaterial, GemBase)

    public:

        //////////
        // Constructor
        GEMglColorMaterial (t_symbol*, t_symbol*);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglColorMaterial();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum face;		// VAR
	virtual void	faceMess 	(int);		// FUN glColorMaterial GLenum

	GLenum mode;		// VAR
	virtual void	modeMess 	(int);		// FUN glColorMaterial GLenum


    private:

        //////////
        // Static member functions

	static void	faceMessCallback	(void*, t_symbol*);		// CALLBACK glColorMaterial
	static void	modeMessCallback	(void*, t_symbol*);		// CALLBACK glColorMaterial

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
