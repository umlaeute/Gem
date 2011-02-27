/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A cone

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_CONE_H_
#define INCLUDE_CONE_H_

#include "cylinder.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    cone
    
    Creates a cone

KEYWORD
    geo
    
DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN cone : public cylinder
{
    CPPEXTERN_HEADER(cone, cylinder);

    public:

	    //////////
	    // Constructor
  cone(t_floatarg size,t_floatarg slice);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~cone();

    	//////////
    	// Do the rendering
	virtual void setupParameters(void);
};

#endif	// for header file
