/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glColor3usv(GLushort* v)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCOLOR3USV_H_
#define INCLUDE_GEM_GLCOLOR3USV_H_

#include "Base/GemBase.h"
#include "Base/GemGLUtil.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglColor3usv

    A Wrapper for the openGL-command "glColor3usv(GLushort* v)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglColor3usv : public GemBase
{
    CPPEXTERN_HEADER(GEMglColor3usv, GemBase)

    public:

        //////////
        // Constructor
        GEMglColor3usv (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
    virtual ~GEMglColor3usv();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLushort m_v[3];		// VAR
	virtual void	vMess 	(t_float, t_float, t_float);		// FUN glColor3usv GLushort*


    private:

        //////////
        // Static member functions

	static void	vMessCallback	(void*, t_floatarg, t_floatarg, t_floatarg);		// CALLBACK glColor3usv

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
