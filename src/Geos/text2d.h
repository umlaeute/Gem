/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A text2d

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_TEXT2D_H_
#define INCLUDE_TEXT2D_H_

#include "Base/TextBase.h"

#ifdef FTGL
# include "FTGLBitmapFont.h"
# include "FTGLPixmapFont.h"
#elif defined GLTT
#include "GLTTBitmapFont.h"
# if defined __linux__ || defined __APPLE__
# include "GLTTPixmapFont.h"
# endif
#endif

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    text2d
    
    Creates a text2d string

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN text2d : public TextBase
{
    CPPEXTERN_HEADER(text2d, TextBase)

    public:

		//////////
		// Constructor
    	text2d(int argc, t_atom *argv);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~text2d();

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);

	//////////
	// Set the font size
	virtual void	setFontSize(int size);

	//////////
	// The font to use
	virtual void  fontNameMess(const char *filename);

	//-----------------------------------
	// GROUP:	Member variables
	//-----------------------------------
#ifdef FTGL
    	FTGLPixmapFont	*m_pfont;
    	FTGLBitmapFont	*m_bfont;
#elif defined GLTT
	////////
	// make the actual fonts
	virtual int     makeFontFromFace();

	//////////
    	// The font structure
#if defined __linux__ || defined __APPLE__
    	GLTTPixmapFont		*m_pfont;
#endif
    	GLTTBitmapFont		*m_bfont;
#endif
	//////
	// anti aliasing (aka: pixmap instead of bitmap)
	int 		m_antialias;
	void            aliasMess(float size);
 private:
	static void     aliasMessCallback(void *data, t_floatarg tog);
};

#endif	// for header file
