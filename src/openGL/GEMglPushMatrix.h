/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glPushMatrix(void)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLPUSHMATRIX_H_
#define INCLUDE_GEM_GLPUSHMATRIX_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglPushMatrix

    A Wrapper for the openGL-command "glPushMatrix(void)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglPushMatrix : public GemBase
{
    CPPEXTERN_HEADER(GEMglPushMatrix, GemBase)

    public:

        //////////
        // Constructor
        GEMglPushMatrix ();         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglPushMatrix();

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
