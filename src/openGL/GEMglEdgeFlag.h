/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glEdgeFlag(GLboolean flag)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLEDGEFLAG_H_
#define INCLUDE_GEM_GLEDGEFLAG_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglEdgeFlag

    A Wrapper for the openGL-command "glEdgeFlag(GLboolean flag)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglEdgeFlag : public GemBase
{
    CPPEXTERN_HEADER(GEMglEdgeFlag, GemBase)

    public:

        //////////
        // Constructor
        GEMglEdgeFlag (t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglEdgeFlag();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLboolean flag;		// VAR
	virtual void	flagMess 	(int);		// FUN glEdgeFlag GLboolean


    private:

        //////////
        // Static member functions

	static void	flagMessCallback	(void*, t_floatarg);		// CALLBACK glEdgeFlag

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
