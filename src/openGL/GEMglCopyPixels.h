/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glCopyPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLCOPYPIXELS_H_
#define INCLUDE_GEM_GLCOPYPIXELS_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglCopyPixels

    A Wrapper for the openGL-command "glCopyPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglCopyPixels : public GemBase
{
    CPPEXTERN_HEADER(GEMglCopyPixels, GemBase)

    public:

    //////////
    // Constructor
    GEMglCopyPixels (int argc, t_atom* argv);

    protected:

        //////////
        // Destructor
        virtual ~GEMglCopyPixels();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLint x;		// VAR
	virtual void	xMess 	(int);		// FUN glCopyPixels GLint

	GLint y;		// VAR
	virtual void	yMess 	(int);		// FUN glCopyPixels GLint

	GLsizei width;		// VAR
	virtual void	widthMess 	(int);		// FUN glCopyPixels GLsizei

	GLsizei height;		// VAR
	virtual void	heightMess 	(int);		// FUN glCopyPixels GLsizei

	GLenum type;		// VAR
	virtual void	typeMess 	(int);		// FUN glCopyPixels GLenum


    private:

        //////////
        // Static member functions

	static void	xMessCallback	(void*, t_floatarg);		// CALLBACK glCopyPixels
	static void	yMessCallback	(void*, t_floatarg);		// CALLBACK glCopyPixels
	static void	widthMessCallback	(void*, t_floatarg);		// CALLBACK glCopyPixels
	static void	heightMessCallback	(void*, t_floatarg);		// CALLBACK glCopyPixels
	static void	typeMessCallback	(void*, t_symbol*);		// CALLBACK glCopyPixels

	// we need some inlets
	t_inlet	*m_inlet[ 5 ];
};

#endif  // for header file
