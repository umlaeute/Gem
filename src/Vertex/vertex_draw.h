/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A vertex_draw

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) G¸nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f¸r::uml‰ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_vertex_draw_H_
#define INCLUDE_vertex_draw_H_

#include "Base/GemVertex.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    vertex_draw
    
    Creates a vertex_draw

KEYWORDS
    geo
    
DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN vertex_draw : public GemBase
{
    CPPEXTERN_HEADER(vertex_draw, GemBase)

    public:

        //////////
        // Constructor
    	vertex_draw(t_floatarg size);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~vertex_draw();
        
        int	m_vao, m_color, m_texcoord;
        int	*m_index;
        float *vertices;
        int	m_oldsize;
        int	m_init;
        int 	m_defaultDraw;

        
    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);


	virtual void    typeMess(t_symbol*s);
	GLint m_drawType;

 private:
        static void 	blendMessCallback(void *data, t_floatarg size);
        static void 	colorMessCallback(void *data, t_floatarg size);
        static void 	texcoordMessCallback(void *data, t_floatarg t);
        static void 	typeMessCallback(void *data, t_symbol*s);

};

#endif	// for header file
