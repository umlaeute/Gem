/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Pixel color inversion

    Copyright (c) 1997-1998 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_INVERT_H_
#define INCLUDE_PIX_INVERT_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_invert
    
    Pixel color inversion

KEYWORDS
    pix
    
DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN pix_invert : public GemPixObj
{
    CPPEXTERN_HEADER(pix_invert, GemPixObj)

    public:

	    //////////
	    // Constructor
    	pix_invert();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_invert();

    	//////////
    	// Do the processing
    	virtual void 	processImage(imageStruct &image);

    	//////////
    	// This is called whenever a new gray8 image comes through.
    	virtual void 	processGrayImage(imageStruct &image);
};

#endif	// for header file
