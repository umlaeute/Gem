/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCOLORMASK_H_
#define INCLUDE_GEM_GLCOLORMASK_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglColorMask

    A Wrapper for the openGL-command "glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglColorMask : public GemBase
{
    CPPEXTERN_HEADER(GEMglColorMask, GemBase)

    public:

        //////////
        // Constructor
        GEMglColorMask (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglColorMask();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLboolean red;		// VAR
	virtual void	redMess 	(int);		// FUN glColorMask GLboolean

	GLboolean green;		// VAR
	virtual void	greenMess 	(int);		// FUN glColorMask GLboolean

	GLboolean blue;		// VAR
	virtual void	blueMess 	(int);		// FUN glColorMask GLboolean

	GLboolean alpha;		// VAR
	virtual void	alphaMess 	(int);		// FUN glColorMask GLboolean


    private:

        //////////
        // Static member functions

	static void	redMessCallback	(void*, t_floatarg);		// CALLBACK glColorMask
	static void	greenMessCallback	(void*, t_floatarg);		// CALLBACK glColorMask
	static void	blueMessCallback	(void*, t_floatarg);		// CALLBACK glColorMask
	static void	alphaMessCallback	(void*, t_floatarg);		// CALLBACK glColorMask

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
