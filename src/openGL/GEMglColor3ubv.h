/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glColor3ubv(GLubyte* v)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCOLOR3UBV_H_
#define INCLUDE_GEM_GLCOLOR3UBV_H_

#include "Base/GemBase.h"
#include "Base/GemGLUtil.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglColor3ubv

    A Wrapper for the openGL-command "glColor3ubv(GLubyte* v)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglColor3ubv : public GemBase
{
    CPPEXTERN_HEADER(GEMglColor3ubv, GemBase)

    public:

        //////////
        // Constructor
        GEMglColor3ubv (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
    virtual ~GEMglColor3ubv();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLubyte m_v[3];		// VAR
	virtual void	vMess 	(t_float, t_float, t_float);		// FUN glColor3ubv GLubyte*


    private:

        //////////
        // Static member functions

	static void	vMessCallback	(void*, t_floatarg, t_floatarg, t_floatarg);		// CALLBACK glColor3ubv

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
