/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glClearIndex(GLfloat c)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCLEARINDEX_H_
#define INCLUDE_GEM_GLCLEARINDEX_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglClearIndex

    A Wrapper for the openGL-command "glClearIndex(GLfloat c)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglClearIndex : public GemBase
{
    CPPEXTERN_HEADER(GEMglClearIndex, GemBase)

    public:

        //////////
        // Constructor
        GEMglClearIndex (t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglClearIndex();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLfloat c;		// VAR
	virtual void	cMess 	(t_float);		// FUN glClearIndex GLfloat


    private:

        //////////
        // Static member functions

	static void	cMessCallback	(void*, t_floatarg);		// CALLBACK glClearIndex

	// we need some inlets
	t_inlet	*m_inlet[ 1 ];
};

#endif  // for header file
