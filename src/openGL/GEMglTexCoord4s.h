/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glTexCoord4s(GLshort s, GLshort t, GLshort r, GLshort q)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLTEXCOORD4S_H_
#define INCLUDE_GEM_GLTEXCOORD4S_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglTexCoord4s

    A Wrapper for the openGL-command "glTexCoord4s(GLshort s, GLshort t, GLshort r, GLshort q)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglTexCoord4s : public GemBase
{
    CPPEXTERN_HEADER(GEMglTexCoord4s, GemBase)

    public:

        //////////
        // Constructor
        GEMglTexCoord4s (t_floatarg, t_floatarg, t_floatarg, t_floatarg);         // CON

    protected:

        //////////
        // Destructor
        virtual ~GEMglTexCoord4s();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLshort s;		// VAR
	virtual void	sMess 	(int);		// FUN glTexCoord4s GLshort

	GLshort t;		// VAR
	virtual void	tMess 	(int);		// FUN glTexCoord4s GLshort

	GLshort r;		// VAR
	virtual void	rMess 	(int);		// FUN glTexCoord4s GLshort

	GLshort q;		// VAR
	virtual void	qMess 	(int);		// FUN glTexCoord4s GLshort


    private:

        //////////
        // Static member functions

	static void	sMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord4s
	static void	tMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord4s
	static void	rMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord4s
	static void	qMessCallback	(void*, t_floatarg);		// CALLBACK glTexCoord4s

	// we need some inlets
	t_inlet	*m_inlet[ 4 ];
};

#endif  // for header file
