/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glRecti(GLint x1, GLint y1, GLint x2, GLint y2)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLRECTI_H_
#define INCLUDE_GEM_GLRECTI_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglRecti

    A Wrapper for the openGL-command "glRecti(GLint x1, GLint y1, GLint x2, GLint y2)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglRecti : public GemBase
{
    CPPEXTERN_HEADER(GEMglRecti, GemBase)

    public:

        //////////
        // Constructor
        GEMglRecti (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglRecti();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLint x1;		// VAR
	virtual void	x1Mess 	(int);		// FUN glRecti GLint

	GLint y1;		// VAR
	virtual void	y1Mess 	(int);		// FUN glRecti GLint

	GLint x2;		// VAR
	virtual void	x2Mess 	(int);		// FUN glRecti GLint

	GLint y2;		// VAR
	virtual void	y2Mess 	(int);		// FUN glRecti GLint


    private:

        //////////
        // Static member functions

	static void	x1MessCallback	(void*, t_floatarg);		// CALLBACK glRecti
	static void	y1MessCallback	(void*, t_floatarg);		// CALLBACK glRecti
	static void	x2MessCallback	(void*, t_floatarg);		// CALLBACK glRecti
	static void	y2MessCallback	(void*, t_floatarg);		// CALLBACK glRecti

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
