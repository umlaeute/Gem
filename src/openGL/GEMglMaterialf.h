/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glMaterialf(GLenum face, GLenum pname, GLfloat param)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLMATERIALF_H_
#define INCLUDE_GEM_GLMATERIALF_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglMaterialf

    A Wrapper for the openGL-command "glMaterialf(GLenum face, GLenum pname, GLfloat param)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglMaterialf : public GemBase
{
    CPPEXTERN_HEADER(GEMglMaterialf, GemBase)

    public:

        //////////
        // Constructor
        GEMglMaterialf (t_symbol*, t_symbol*, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglMaterialf();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum face;		// VAR
	virtual void	faceMess 	(int);		// FUN glMaterialf GLenum

	GLenum pname;		// VAR
	virtual void	pnameMess 	(int);		// FUN glMaterialf GLenum

	GLfloat param;		// VAR
	virtual void	paramMess 	(t_float);		// FUN glMaterialf GLfloat


    private:

        //////////
        // Static member functions

	static void	faceMessCallback	(void*, t_symbol*);		// CALLBACK glMaterialf
	static void	pnameMessCallback	(void*, t_symbol*);		// CALLBACK glMaterialf
	static void	paramMessCallback	(void*, t_floatarg);		// CALLBACK glMaterialf

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
