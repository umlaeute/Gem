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

class GLTTFont;
class FTFace;

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
		virtual void	setPrecision(float prec);

	    //-----------------------------------
	    // GROUP:	Member variables
	    //-----------------------------------
    
	   	//////////
    	// The font structure
    	GLTTFont		*m_font;

    	//////////
    	// The font structure
    	FTFace 			*m_face;
};

#endif	// for header file
