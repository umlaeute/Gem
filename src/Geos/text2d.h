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

class GLTTBitmapFont;
class GLTTPixmapFont;
class FTFace;

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
    	// The font to use
    	virtual void   	fontNameMess(const char *filename);

		//////////
		// Create the actual font from the face
		int				makeFontFromFace();

		//////////
		// Set the font size
		virtual void	setFontSize(int size);

		//////////
		// Set the precision for rendering
		// This is a no-op
		virtual void	setPrecision(float)			{ }

	    //-----------------------------------
	    // GROUP:	Member variables
	    //-----------------------------------
    
	   	//////////
    	// The font structure

    	GLTTBitmapFont	*m_bfont;
    	GLTTPixmapFont	*m_pfont;

    	//////////
    	// The font structure

	int m_antialias;
    	FTFace 			*m_face;

	void text2d :: aliasMess(float size);
	static void text2d :: aliasMessCallback(void *data, t_floatarg tog);
};

#endif	// for header file
