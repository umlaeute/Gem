/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    The state to pass among GEM objects

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_GEMCACHE_H_
#define INCLUDE_GEMCACHE_H_

#include "Base/GemExportDef.h"

class gemhead;

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GemCache
    
    The cache to pass among GEM objects

DESCRIPTION
    
-----------------------------------------------------------------*/
class GEM_EXTERN GemCache
{
    public:

    	//////////
    	// Was a modification made which will void a display list?
    	int 	    	    dirty;
    	
    	//////////
    	// Should the image be resent?
    	int 	    	    resendImage;

    	//////////
    	// The DAG is incorrect and the rendering in the 
    	//  	chain should stop.
    	void	    	    breakDAG();
    	
        //////////
        // Constructor
    	GemCache(gemhead *parent);
    	
        //////////
        // Destructor
        ~GemCache();

        //////////
    	gemhead     	    *m_parent;
};

#endif	// for header file
