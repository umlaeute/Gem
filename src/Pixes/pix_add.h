/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Add two images together.

    Copyright (c) 1997-1998 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_ADD_H_
#define INCLUDE_PIX_ADD_H_

#include "Base/GemPixDualObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_add
    
    Add two images together.

KEYWORDS
    pix
    
DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN pix_add : public GemPixDualObj
{
    CPPEXTERN_HEADER(pix_add, GemPixDualObj)

    public:

	    //////////
    	// Constructor
    	pix_add();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_add();

    	//////////
    	// Do the processing
    	virtual void 	processDualImage(imageStruct &image, imageStruct &right);

    	//////////
    	// Do the processing
    	virtual void 	processDualGray(imageStruct &image, imageStruct &right);

    	//////////
    	// Do the processing
    	virtual void 	processRightGray(imageStruct &image, imageStruct &right);
};

#endif	// for header file
