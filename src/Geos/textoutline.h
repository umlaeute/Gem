/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A textoutline

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_TEXTOUTLINE_H_
#define INCLUDE_TEXTOUTLINE_H_

#include "Base/TextBase.h"

#ifdef GLTT
#include "GLTTOutlineFont.h"
#endif

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    textoutline
    
    Creates a textoutline string

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN textoutline : public TextBase
{
    CPPEXTERN_HEADER(textoutline, TextBase)

    public:

		//////////
		// Constructor
    	textoutline(int argc, t_atom *argv);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~textoutline();

#ifdef FTGL
	virtual FTFont*makeFont(const char*fontname);
#elif defined GLTT
	virtual void    destroyFont();
	virtual int     makeFontFromFace();
	//////////
    	// The font structure
    	GLTTOutlineFont		*m_font;

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);
#endif
};

#endif	// for header file
