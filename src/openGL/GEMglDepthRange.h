/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glDepthRange(GLclampd near_val, GLclampd far_val)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLDEPTHRANGE_H_
#define INCLUDE_GEM_GLDEPTHRANGE_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglDepthRange

    A Wrapper for the openGL-command "glDepthRange(GLclampd near_val, GLclampd far_val)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglDepthRange : public GemBase
{
    CPPEXTERN_HEADER(GEMglDepthRange, GemBase)

    public:

        //////////
        // Constructor
        GEMglDepthRange (t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglDepthRange();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLclampd near_val;		// VAR
	virtual void	near_valMess 	(double);		// FUN glDepthRange GLclampd

	GLclampd far_val;		// VAR
	virtual void	far_valMess 	(double);		// FUN glDepthRange GLclampd


    private:

        //////////
        // Static member functions

	static void	near_valMessCallback	(void*, t_floatarg);		// CALLBACK glDepthRange
	static void	far_valMessCallback	(void*, t_floatarg);		// CALLBACK glDepthRange

	// we need some inlets
	t_inlet	*m_inlet[ 2 ];
};

#endif  // for header file
