/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A text3d

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_TEXT3D_H_
#define INCLUDE_TEXT3D_H_

#include "Base/TextBase.h"

#ifdef GLTT
#include "GLTTFont.h"
#endif

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    text3d
    
    Creates a text3d string

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN text3d : public TextBase
{
    CPPEXTERN_HEADER(text3d, TextBase)

    public:

		//////////
		// Constructor
    	text3d(int argc, t_atom *argv);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~text3d();

#ifdef FTGL
	virtual FTFont*makeFont(const char*fontname);
#elif defined GLTT
	virtual int     makeFontFromFace();
	virtual void    destroyFont();
	//////////
    	// The font structure
    	GLTTFont		*m_font;

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);
#endif
};

#endif	// for header file
