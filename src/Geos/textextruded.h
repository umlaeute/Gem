/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A textextruded

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_TEXTEXTRUDED_H_
#define INCLUDE_TEXTEXTRUDED_H_

#include "Base/TextBase.h"

#ifdef GLTT
#include "GLTTFont.h"
#endif

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    textextruded
    
    Creates a textextruded string

DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN textextruded : public TextBase
{
    CPPEXTERN_HEADER(textextruded, TextBase)

    public:

		//////////
		// Constructor
    	textextruded(int argc, t_atom *argv);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~textextruded();

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);

	//-----------------------------------
	// GROUP:	Member variables
	//-----------------------------------
#ifdef GLTT
	virtual int     makeFontFromFace();

	//////////
    	// The font structure
    	GLTTFont		*m_font;
#endif
	//////////
	// Set the precision for rendering
	virtual void	setDepth(float depth);
 private:
	static void 	depthMessCallback(void *data, t_floatarg depth);

};

#endif	// for header file
