/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glRectd(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLRECTD_H_
#define INCLUDE_GEM_GLRECTD_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglRectd

    A Wrapper for the openGL-command "glRectd(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglRectd : public GemBase
{
    CPPEXTERN_HEADER(GEMglRectd, GemBase)

    public:

        //////////
        // Constructor
        GEMglRectd (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglRectd();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLdouble x1;		// VAR
	virtual void	x1Mess 	(double);		// FUN glRectd GLdouble

	GLdouble y1;		// VAR
	virtual void	y1Mess 	(double);		// FUN glRectd GLdouble

	GLdouble x2;		// VAR
	virtual void	x2Mess 	(double);		// FUN glRectd GLdouble

	GLdouble y2;		// VAR
	virtual void	y2Mess 	(double);		// FUN glRectd GLdouble


    private:

        //////////
        // Static member functions

	static void	x1MessCallback	(void*, t_floatarg);		// CALLBACK glRectd
	static void	y1MessCallback	(void*, t_floatarg);		// CALLBACK glRectd
	static void	x2MessCallback	(void*, t_floatarg);		// CALLBACK glRectd
	static void	y2MessCallback	(void*, t_floatarg);		// CALLBACK glRectd

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
