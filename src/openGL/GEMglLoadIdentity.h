/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glLoadIdentity(void)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLLOADIDENTITY_H_
#define INCLUDE_GEM_GLLOADIDENTITY_H_

#include "Base/GemBase.h"
//#include "Base/GemGLUtil.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglLoadIdentity

    A Wrapper for the openGL-command "glLoadIdentity(void)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglLoadIdentity : public GemBase
{
    CPPEXTERN_HEADER(GEMglLoadIdentity, GemBase)

    public:

        //////////
        // Constructor
        GEMglLoadIdentity ();         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglLoadIdentity();

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
