/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A cube

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_CUBE_H_
#define INCLUDE_CUBE_H_

#include "Base/GemShape.h"
#include <string.h>

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    cube
    
    Creates a cube

KEYWORDS
    geo

DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN cube : public GemShape
{
    CPPEXTERN_HEADER(cube, GemShape)

    public:

	    //////////
	    // Constructor
    	cube(t_floatarg size);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~cube();

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);
	
	int	m_blend;
	
    private:
    //////////
    // Static member functions
    static void 	blendMessCallback(void *data, t_floatarg size);
};

#endif	// for header file
