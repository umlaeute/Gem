/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glIndexs(GLshort c)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLINDEXS_H_
#define INCLUDE_GEM_GLINDEXS_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglIndexs

    A Wrapper for the openGL-command "glIndexs(GLshort c)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglIndexs : public GemBase
{
    CPPEXTERN_HEADER(GEMglIndexs, GemBase)

    public:

        //////////
        // Constructor
        GEMglIndexs (t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglIndexs();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLshort c;		// VAR
	virtual void	cMess 	(int);		// FUN glIndexs GLshort


    private:

        //////////
        // Static member functions

	static void	cMessCallback	(void*, t_floatarg);		// CALLBACK glIndexs

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
