/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A vertex_combine

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) G¸nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f¸r::uml‰ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_vertex_combine_H_
#define INCLUDE_vertex_combine_H_

#include "Base/GemVertex.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    vertex_combine
    
    Creates a vertex_combine

KEYWORDS
    geo
    
DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN vertex_combine : public GemBase
{
    CPPEXTERN_HEADER(vertex_combine, GemBase)

    public:

        //////////
        // Constructor
    	vertex_combine();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~vertex_combine();
        
        float	m_x,m_y,m_z,m_w;
        float	*m_rightVertexArray;
        float	*m_rightColorArray;
        float	m_blend;
        int 	m_previousSize;
        int	m_vertNum,m_vertCount;
        int	m_vertCountR;
        t_inlet *m_inlet;
        GemCache *m_cacheRight;

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);
        virtual void 	postrender(GemState *state);
        virtual void 	rightRender(GemState *state);
        
        //private:
        static void 	gem_rightMessCallback(void *x, t_symbol *s, int argc, t_atom *argv);
        static void	blendCallback(void *data, t_floatarg x);

};

#endif	// for header file
