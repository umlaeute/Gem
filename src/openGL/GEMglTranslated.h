/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTranslated(GLdouble x, GLdouble y, GLdouble z)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTRANSLATED_H_
#define INCLUDE_GEM_GLTRANSLATED_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTranslated

    A Wrapper for the openGL-command "glTranslated(GLdouble x, GLdouble y, GLdouble z)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTranslated : public GemBase
{
    CPPEXTERN_HEADER(GEMglTranslated, GemBase)

    public:

        //////////
        // Constructor
        GEMglTranslated (t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTranslated();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLdouble x;		// VAR
	virtual void	xMess 	(double);		// FUN glTranslated GLdouble

	GLdouble y;		// VAR
	virtual void	yMess 	(double);		// FUN glTranslated GLdouble

	GLdouble z;		// VAR
	virtual void	zMess 	(double);		// FUN glTranslated GLdouble


    private:

        //////////
        // Static member functions

	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glTranslated
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glTranslated
	static void	zMessCallback	(void*, t_floatarg);		// CALLBACK glTranslated

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
