/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A triangle

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_TRIANGLE_H_
#define INCLUDE_TRIANGLE_H_

#include "Base/GemShape.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    triangle
    
    Creates a triangle

KEYWORDS
    geo

DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN triangle : public GemShape
{
    CPPEXTERN_HEADER(triangle, GemShape)

    public:

        //////////
        // Constructor
    	triangle(t_floatarg size);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~triangle();

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);
};

#endif	// for header file
