/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexGenf(GLenum coord, GLenum pname, GLfloat param)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXGENF_H_
#define INCLUDE_GEM_GLTEXGENF_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexGenf

    A Wrapper for the openGL-command "glTexGenf(GLenum coord, GLenum pname, GLfloat param)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexGenf : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexGenf, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexGenf (t_symbol*, t_symbol*, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexGenf();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLenum coord;		// VAR
	virtual void	coordMess 	(int);		// FUN glTexGenf GLenum

	GLenum pname;		// VAR
	virtual void	pnameMess 	(int);		// FUN glTexGenf GLenum

	GLfloat param;		// VAR
	virtual void	paramMess 	(t_float);		// FUN glTexGenf GLfloat


    private:

        //////////
        // Static member functions

	static void	coordMessCallback	(void*, t_symbol*);		// CALLBACK glTexGenf
	static void	pnameMessCallback	(void*, t_symbol*);		// CALLBACK glTexGenf
	static void	paramMessCallback	(void*, t_floatarg);		// CALLBACK glTexGenf

	// we need some inlets
	t_inlet	*m_inlet[ 3 ];
};

#endif  // for header file
