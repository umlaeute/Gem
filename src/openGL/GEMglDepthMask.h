/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glDepthMask(GLboolean flag)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLDEPTHMASK_H_
#define INCLUDE_GEM_GLDEPTHMASK_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglDepthMask

    A Wrapper for the openGL-command "glDepthMask(GLboolean flag)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglDepthMask : public GemBase
{
    CPPEXTERN_HEADER(GEMglDepthMask, GemBase)

    public:

        //////////
        // Constructor
        GEMglDepthMask (t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglDepthMask();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLboolean flag;		// VAR
	virtual void	flagMess 	(int);		// FUN glDepthMask GLboolean


    private:

        //////////
        // Static member functions

	static void	flagMessCallback	(void*, t_floatarg);		// CALLBACK glDepthMask

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
