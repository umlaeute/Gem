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

#ifndef INCLUDE_VERTEX_SUB_H_
#define INCLUDE_VERTEX_SUB_H_

#include "vertex_add.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    vertex_sub
    
    Creates a vertex_sub

KEYWORDS
    geo
    
DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN vertex_sub : public vertex_add
{
    CPPEXTERN_HEADER(vertex_sub, vertex_add)

    public:

        //////////
        // Constructor
  vertex_sub(int, t_atom*);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~vertex_sub();
	
    	//////////
    	// Do the rendering
	virtual void    vertexProcess(int lsize, float*larray, int rsize, float*rarray);
};

#endif	// for header file
