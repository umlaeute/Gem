/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

A wrapper for "glBitmap(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, GLubyte* bitmap)"

  Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
  this file has been generated automatically...

  For information on usage and redistribution, and for a DISCLAIMER OF ALL
  WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

 -----------------------------------------------------------------*/

#ifndef INCLUDE_GEM_GLBITMAP_H_
#define INCLUDE_GEM_GLBITMAP_H_

#include "Base/GemGLUtil.h"
#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GEMglBitmap

    A Wrapper for the openGL-command "glBitmap(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, GLubyte* bitmap)"

KEYWORDS
    openGL

OPENGL_VERSION 0

------------------------------------------------------------------*/

class GEM_EXTERN GEMglBitmap : public GemBase
{
    CPPEXTERN_HEADER(GEMglBitmap, GemBase)

    public:

    //////////
    // Constructor
    GEMglBitmap (int argc, t_atom* argv);

    protected:

        //////////
        // Destructor
        virtual ~GEMglBitmap();

        //////////
        // Do the rendering
        virtual void    render (GemState *state);

       //////////
       // define and set the variables

	 GLsizei width;		// VAR
	virtual void	widthMess 	(int);		// FUN glBitmap GLsizei

	GLsizei height;		// VAR
	virtual void	heightMess 	(int);		// FUN glBitmap GLsizei

	GLfloat xorig;		// VAR
	virtual void	xorigMess 	(t_float);		// FUN glBitmap GLfloat

	GLfloat yorig;		// VAR
	virtual void	yorigMess 	(t_float);		// FUN glBitmap GLfloat

	GLfloat xmove;		// VAR
	virtual void	xmoveMess 	(t_float);		// FUN glBitmap GLfloat

	GLfloat ymove;		// VAR
	virtual void	ymoveMess 	(t_float);		// FUN glBitmap GLfloat

	GLubyte* bitmap;		// VAR


    private:

        //////////
        // Static member functions

	static void	widthMessCallback	(void*, t_floatarg);		// CALLBACK glBitmap
	static void	heightMessCallback	(void*, t_floatarg);		// CALLBACK glBitmap
	static void	xorigMessCallback	(void*, t_floatarg);		// CALLBACK glBitmap
	static void	yorigMessCallback	(void*, t_floatarg);		// CALLBACK glBitmap
	static void	xmoveMessCallback	(void*, t_floatarg);		// CALLBACK glBitmap
	static void	ymoveMessCallback	(void*, t_floatarg);		// CALLBACK glBitmap

	// we need some inlets
	t_inlet	*m_inlet[ 7 ];
};

#endif  // for header file
