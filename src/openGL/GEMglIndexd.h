/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glIndexd(GLdouble c)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLINDEXD_H_
#define INCLUDE_GEM_GLINDEXD_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglIndexd

    A Wrapper for the openGL-command "glIndexd(GLdouble c)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglIndexd : public GemBase
{
    CPPEXTERN_HEADER(GEMglIndexd, GemBase)

    public:

        //////////
        // Constructor
        GEMglIndexd (t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglIndexd();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLdouble c;		// VAR
	virtual void	cMess 	(double);		// FUN glIndexd GLdouble


    private:

        //////////
        // Static member functions

	static void	cMessCallback	(void*, t_floatarg);		// CALLBACK glIndexd

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
