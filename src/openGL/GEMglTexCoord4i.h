/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexCoord4i(GLint s, GLint t, GLint r, GLint q)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXCOORD4I_H_
#define INCLUDE_GEM_GLTEXCOORD4I_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexCoord4i

    A Wrapper for the openGL-command "glTexCoord4i(GLint s, GLint t, GLint r, GLint q)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexCoord4i : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexCoord4i, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexCoord4i (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexCoord4i();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLint s;		// VAR
	virtual void	sMess 	(int);		// FUN glTexCoord4i GLint

	GLint t;		// VAR
	virtual void	tMess 	(int);		// FUN glTexCoord4i GLint

	GLint r;		// VAR
	virtual void	rMess 	(int);		// FUN glTexCoord4i GLint

	GLint q;		// VAR
	virtual void	qMess 	(int);		// FUN glTexCoord4i GLint


    private:

        //////////
        // Static member functions

	static void	sMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord4i
	static void	tMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord4i
	static void	rMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord4i
	static void	qMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord4i

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
