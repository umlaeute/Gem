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

#ifndef INCLUDE_VERTEX_VBR_H_
#define INCLUDE_VERTEX_VBR_H_

#include "vertex_add.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    vertex_VBR
    
    Creates a vertex_VBR

KEYWORDS
    geo
    
DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN vertex_VBR : public vertex_add
{
    CPPEXTERN_HEADER(vertex_VBR, vertex_add)

    public:

        //////////
        // Constructor
  vertex_VBR(int, t_atom*);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~vertex_VBR();
	
    	//////////
    	// Do the rendering
	virtual void    vertexProcess(int lsize, float*larray, int rsize, float*rarray);

	virtual void    rightRender(GemState*, GemCache*);
};

#endif	// for header file
