/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A square

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_SQUARE_H_
#define INCLUDE_SQUARE_H_

#include "Base/GemShape.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    square
    
    Creates a square

KEYWORDS
    geo
    
DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN square : public GemShape
{
    CPPEXTERN_HEADER(square, GemShape)

    public:

        //////////
        // Constructor
    	square(t_floatarg size);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~square();
        
        int	m_blend;

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);
        static void 	blendMessCallback(void *data, t_floatarg size);

};

#endif	// for header file
