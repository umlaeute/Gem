/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A vertex_grid

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) G¸nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f¸r::uml‰ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_vertex_grid_H_
#define INCLUDE_vertex_grid_H_

#include "Base/GemVertex.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    vertex_grid
    
    Creates a vertex_grid

KEYWORDS
    geo
    
DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN vertex_grid : public GemShape
{
    CPPEXTERN_HEADER(vertex_grid, GemShape)

    public:

        //////////
        // Constructor
    	vertex_grid(t_floatarg size);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~vertex_grid();
        
        int	m_x,m_y,m_oldx,m_oldy;
        float	m_spacex, m_spacey;
        float 	maxX,maxY;
        float 	ratioX, ratioY;
        float	*m_ColorArray;
        float	*m_VertexArray;
        float	*m_TexCoordArray;

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);
        static void 	sizeMessCallback(void *data, t_floatarg x, t_floatarg y);
        static void 	spacingMessCallback(void *data, t_floatarg x, t_floatarg y);

};

#endif	// for header file
