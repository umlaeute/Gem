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

#ifndef INCLUDE_VERTEX_MUL_H_
#define INCLUDE_VERTEX_MUL_H_

#include "vertex_add.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    vertex_mul
    
    Creates a vertex_mul

KEYWORDS
    geo
    
DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN vertex_mul : public vertex_add
{
    CPPEXTERN_HEADER(vertex_mul, vertex_add)

    public:

        //////////
        // Constructor
  vertex_mul(int, t_atom*);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~vertex_mul();
	
    	//////////
    	// Do the rendering
	virtual void    vertexProcess(int lsize, float*larray, int rsize, float*rarray);
};

#endif	// for header file
