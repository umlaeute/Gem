/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A color_set

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) G¸nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f¸r::uml‰ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_color_set_H_
#define INCLUDE_color_set_H_

#include "Base/GemShape.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    color_set
    
    Creates a color_set

KEYWORDS
    geo
    
DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN color_set : public GemShape
{
    CPPEXTERN_HEADER(color_set, GemShape)

    public:

        //////////
        // Constructor
    	color_set(t_floatarg size);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~color_set();
        
        float	m_Red,m_Green,m_Blue,m_Alpha;
        int 	m_previousSize;
        int	m_colorNum,m_colorCount;

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);
        virtual void 	postrender(GemState *state);
        
        static void 	colorMessCallback(void *data, t_floatarg R, t_floatarg G, t_floatarg B, t_floatarg A);
        static void 	numberMessCallback(void *data, t_floatarg num, t_floatarg counter);

};

#endif	// for header file
