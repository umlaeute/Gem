/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glBegin(GLenum mode)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLBEGIN_H_
#define INCLUDE_GEM_GLBEGIN_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglBegin

    A Wrapper for the openGL-command "glBegin(GLenum mode)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglBegin : public GemBase
{
    CPPEXTERN_HEADER(GEMglBegin, GemBase)

    public:

        //////////
        // Constructor
        GEMglBegin (int, t_atom*);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglBegin();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum mode;		// VAR
	 virtual void	modeMess 	(int, t_atom *);//int);		// FUN glBegin GLenum


    private:

        //////////
        // Static member functions

	static void	modeMessCallback	(void*, t_symbol*,int,t_atom*);		// CALLBACK glBegin

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
