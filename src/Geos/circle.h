/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A circle

    Copyright (c) 1997-1998 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_CIRCLE_H_
#define INCLUDE_CIRCLE_H_

#include "Base/GemShape.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    circle
    
    Creates a circle

KEYWORDS
    geo
    
DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN circle : public GemShape
{
    CPPEXTERN_HEADER(circle, GemShape)

    public:

        //////////
        // Constructor
        circle(t_floatarg size);
    	
    protected:
    	
        //////////
        // Destructor
        virtual ~circle();

        //////////
        // Do the rendering
        virtual void 	render(GemState *state);

        //////////
        // cos lookup table
        static GLfloat *m_cos;

        //////////
        // sin lookup table
        static GLfloat *m_sin;
};

#endif	// for header file
