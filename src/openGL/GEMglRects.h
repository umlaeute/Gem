/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glRects(GLshort x1, GLshort y1, GLshort x2, GLshort y2)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLRECTS_H_
#define INCLUDE_GEM_GLRECTS_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglRects

    A Wrapper for the openGL-command "glRects(GLshort x1, GLshort y1, GLshort x2, GLshort y2)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglRects : public GemBase
{
    CPPEXTERN_HEADER(GEMglRects, GemBase)

    public:

        //////////
        // Constructor
        GEMglRects (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglRects();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLshort x1;		// VAR
	virtual void	x1Mess 	(int);		// FUN glRects GLshort

	GLshort y1;		// VAR
	virtual void	y1Mess 	(int);		// FUN glRects GLshort

	GLshort x2;		// VAR
	virtual void	x2Mess 	(int);		// FUN glRects GLshort

	GLshort y2;		// VAR
	virtual void	y2Mess 	(int);		// FUN glRects GLshort


    private:

        //////////
        // Static member functions

	static void	x1MessCallback	(void*, t_floatarg);		// CALLBACK glRects
	static void	y1MessCallback	(void*, t_floatarg);		// CALLBACK glRects
	static void	x2MessCallback	(void*, t_floatarg);		// CALLBACK glRects
	static void	y2MessCallback	(void*, t_floatarg);		// CALLBACK glRects

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
