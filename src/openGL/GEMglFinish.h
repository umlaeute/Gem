/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glFinish(void)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLFINISH_H_
#define INCLUDE_GEM_GLFINISH_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglFinish

    A Wrapper for the openGL-command "glFinish(void)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglFinish : public GemBase
{
    CPPEXTERN_HEADER(GEMglFinish, GemBase)

    public:

        //////////
        // Constructor
        GEMglFinish ();         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglFinish();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables


    private:

        //////////
        // Static member functions


	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
