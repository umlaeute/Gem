/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A vertex_info

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) G¸nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f¸r::uml‰ute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_vertex_info_H_
#define INCLUDE_vertex_info_H_

#include "Base/GemVertex.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    vertex_info
    
    Creates a vertex_info

KEYWORDS
    geo
    
DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN vertex_info : public GemVertex
{
    CPPEXTERN_HEADER(vertex_info, GemVertex)

    public:

        //////////
        // Constructor
    	vertex_info();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~vertex_info();
        
        int 		m_previousSize;
        int		m_vertNum,m_vertCount;
        t_outlet	*m_Vsize;

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);

 private:
        static void 	vertexMessCallback(void *data, t_floatarg num, t_floatarg counter);

};

#endif	// for header file
