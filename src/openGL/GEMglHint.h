/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glHint(GLenum target, GLenum mode)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLHINT_H_
#define INCLUDE_GEM_GLHINT_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglHint

    A Wrapper for the openGL-command "glHint(GLenum target, GLenum mode)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglHint : public GemBase
{
    CPPEXTERN_HEADER(GEMglHint, GemBase)

    public:

        //////////
        // Constructor
        GEMglHint (t_symbol*, t_symbol*);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglHint();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum target;		// VAR
	virtual void	targetMess 	(int);		// FUN glHint GLenum

	GLenum mode;		// VAR
	virtual void	modeMess 	(int);		// FUN glHint GLenum


    private:

        //////////
        // Static member functions

	static void	targetMessCallback	(void*, t_symbol*);		// CALLBACK glHint
	static void	modeMessCallback	(void*, t_symbol*);		// CALLBACK glHint

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
