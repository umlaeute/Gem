/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glClear(GLbitfield mask)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCLEAR_H_
#define INCLUDE_GEM_GLCLEAR_H_

#include "Base/GemBase.h"
#include "Base/GemGLUtil.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglClear

    A Wrapper for the openGL-command "glClear(GLbitfield mask)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglClear : public GemBase
{
    CPPEXTERN_HEADER(GEMglClear, GemBase)

    public:

        //////////
        // Constructor
        GEMglClear (t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglClear();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLbitfield mask;		// VAR
	virtual void	maskMess 	(t_float);		// FUN glClear GLbitfield


    private:

        //////////
        // Static member functions

	static void	maskMessCallback	(void*, t_floatarg);		// CALLBACK glClear

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
