/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glClearDepth(GLclampd depth)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCLEARDEPTH_H_
#define INCLUDE_GEM_GLCLEARDEPTH_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglClearDepth

    A Wrapper for the openGL-command "glClearDepth(GLclampd depth)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglClearDepth : public GemBase
{
    CPPEXTERN_HEADER(GEMglClearDepth, GemBase)

    public:

        //////////
        // Constructor
        GEMglClearDepth (t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglClearDepth();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLclampd depth;		// VAR
	virtual void	depthMess 	(double);		// FUN glClearDepth GLclampd


    private:

        //////////
        // Static member functions

	static void	depthMessCallback	(void*, t_floatarg);		// CALLBACK glClearDepth

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
