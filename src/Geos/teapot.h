/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A teapot

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_TEAPOT_H_
#define INCLUDE_TEAPOT_H_


#if HAVE_LIBGLUT || MACOSX

#include "Base/GemGlutObj.h"


/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
	teapot
    
    Creates a teapot

KEYWORD
    geo
    
DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN teapot : public GemGlutObj
{
    CPPEXTERN_HEADER(teapot, GemGlutObj)

    public:

	    //////////
	    // Constructor
    	teapot(t_floatarg size);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~teapot();

    	//////////
    	// Do the rendering
    	virtual void 	render(GemState *state);
};

#endif
#endif	// for header file
