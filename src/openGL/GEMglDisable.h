/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glDisable(GLenum cap)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLDISABLE_H_
#define INCLUDE_GEM_GLDISABLE_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglDisable

    A Wrapper for the openGL-command "glDisable(GLenum cap)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglDisable : public GemBase
{
    CPPEXTERN_HEADER(GEMglDisable, GemBase)

    public:

        //////////
        // Constructor
        GEMglDisable (t_symbol*);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglDisable();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum cap;		// VAR
	virtual void	capMess 	(int);		// FUN glDisable GLenum


    private:

        //////////
        // Static member functions

	static void	capMessCallback	(void*, t_symbol*);		// CALLBACK glDisable

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
