/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glIndexi(GLint c)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLINDEXI_H_
#define INCLUDE_GEM_GLINDEXI_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglIndexi

    A Wrapper for the openGL-command "glIndexi(GLint c)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglIndexi : public GemBase
{
    CPPEXTERN_HEADER(GEMglIndexi, GemBase)

    public:

        //////////
        // Constructor
        GEMglIndexi (t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglIndexi();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLint c;		// VAR
	virtual void	cMess 	(int);		// FUN glIndexi GLint


    private:

        //////////
        // Static member functions

	static void	cMessCallback	(void*, t_floatarg);		// CALLBACK glIndexi

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
