/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glEnd(void)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLEND_H_
#define INCLUDE_GEM_GLEND_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglEnd

    A Wrapper for the openGL-command "glEnd(void)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglEnd : public GemBase
{
    CPPEXTERN_HEADER(GEMglEnd, GemBase)

    public:

        //////////
        // Constructor
        GEMglEnd ();         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglEnd();

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
