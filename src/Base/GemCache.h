/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    The state to pass among GEM objects

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) G�nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
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
#define GEMCACHE_MAGIC 0x1234567
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
        // Constructor
    	GemCache(gemhead *parent);
    	
        //////////
        // Destructor
        ~GemCache();

	//////////
	// re-set (like creation, but without instantiating
	void reset(gemhead*parent);

        //////////
    	gemhead     	    *m_parent;

	//////////
	// indicates a valid cache
	int m_magic;
};

#endif	// for header file
