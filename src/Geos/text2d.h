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

#ifdef GLTT
#include "GLTTBitmapFont.h"
//# if defined __linux__ || defined __APPLE__
# include "GLTTPixmapFont.h"
//# endif
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

	//////
	// anti aliasing (aka: pixmap instead of bitmap)
	int m_antialias;
	void            aliasMess(int aa);

#ifdef FTGL
	void            setFontSize(t_float size);

	virtual FTFont*	makeFont(const char*fontname);
	FTFont                  *m_afont;
#elif defined GLTT
	virtual void    destroyFont();
	virtual int     makeFontFromFace();
	//////////
    	// The font structure
    	GLTTBitmapFont		*m_font;
    	GLTTPixmapFont		*m_afont;
#endif

 private:
	static void     aliasMessCallback(void *data, t_floatarg aa);

};

#endif	// for header file
