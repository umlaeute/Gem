/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A text

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_TEXTBASE_H_
#define INCLUDE_TEXTBASE_H_

#include "Base/GemBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    TextBase
    
    Base class for text objects

DESCRIPTION

    "ft1" - The font size
    "text" - The text to draw
  
-----------------------------------------------------------------*/
class GEM_EXTERN TextBase : public GemBase
{
    CPPEXTERN_HEADER(TextBase, GemBase)

    public:

		//////////
		// Constructor
    	TextBase(int argc, t_atom *argv);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~TextBase();

    	//////////
    	// Set the text string
    	void	    	textMess(int argc, t_atom *argv);

    	//////////
    	// The font to use
    	virtual void   	fontNameMess(const char *filename) = 0;

		//////////
		// Set the font size
		virtual void	setFontSize(int size) = 0;

		//////////
		// Set the precision for rendering
		virtual void	setPrecision(float prec) = 0;

		//////////
		// The different types of justification
		enum JustifyWidth { LEFT, RIGHT, CENTER };

		//////////
		// The different types of justification
		enum JustifyHeight { BOTTOM, TOP, MIDDLE };

		//////////
		// Set the width justification
		void	setJustification(JustifyWidth wType, JustifyHeight hType);

	    //-----------------------------------
	    // GROUP:	Member variables
	    //-----------------------------------
    
		//////////
		// Do we have a valid font?
		int	m_valid;

    	//////////
    	// The string to display
    	char	    	*m_theString;
    	
    	//////////
    	// The maximum memory currently allocated for the string
		// This includes the terminator /0 !!!
    	int	m_theMaxStringSize;
    	
		//////////
		// The font fize
		int		m_fontSize;
    	
		//////////
		// The rendering precision
		float		m_precision;

		//////////
		// The width justification
		JustifyWidth	m_widthJus;

		//////////
		// The height justification
		JustifyHeight	m_heightJus;

        //////////
        // The inlet
        t_inlet         *m_inlet;

		//////////
		// The default font name
		static char *DEFAULT_FONT;

	    //-----------------------------------
	    // GROUP:	Setup functions
	    //-----------------------------------
    
    private:
    	    
    	//////////
    	// Static member functions
    	static void 	textMessCallback(void *data, t_symbol *, int argc, t_atom *argv);
    	static void 	fontSizeMessCallback(void *data, t_floatarg size);
    	static void 	precisionMessCallback(void *data, t_floatarg prec);
    	static void 	fontNameMessCallback(void *data, t_symbol *s);
    	static void 	justifyMessCallback(void *data, t_symbol *width, t_symbol *height);
};

#endif	// for header file
