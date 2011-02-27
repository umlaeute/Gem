/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    A linear path
    
    Copyright (c) 1997-1998 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_LINEAR_PATH_H_
#define INCLUDE_LINEAR_PATH_H_

#include "Base/GemPathBase.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    linear_path
    

DESCRIPTION
    
    
-----------------------------------------------------------------*/
class GEM_EXTERN linear_path : public GemPathBase
{
    CPPEXTERN_HEADER(linear_path, GemPathBase);

    public:

        //////////
        // Constructor
    	linear_path(int argc, t_atom *argv);
   	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~linear_path();

        //////////
        // When a float val is received
        virtual void    floatMess(float val);
};

#endif	// for header file
