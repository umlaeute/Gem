/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Do a blue screen with pix images.

    Copyright (c) 1997-1998 Mark Danks
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    Copyright (c) 2002 James Tittle & Chris Clepper
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_MASK_H_
#define INCLUDE_PIX_MASK_H_

#include "Base/GemPixDualObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_mask
    
    Do a blue screen with pix images.

KEYWORDS
    pix
    
DESCRIPTION

-----------------------------------------------------------------*/
class GEM_EXTERN pix_mask : public GemPixDualObj
{
    CPPEXTERN_HEADER(pix_mask, GemPixDualObj)

    public:

	    //////////
	    // Constructor
    	pix_mask();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_mask();

    	//////////
    	// Do the processing
    	virtual void 	processDualImage(imageStruct &image, imageStruct &right);

    	//////////
    	//  This is called whenever a new image comes through.
        //	The left image is an RGBA, the right is a gray8
    	virtual void 	processRightGray(imageStruct &image, imageStruct &right);

    	//////////
    	// Do the processing
    	virtual void 	processDualYUV(imageStruct &image, imageStruct &right);
                
    	//////////
    	//  This is called whenever a new image comes through.
        //	The left image is an RGBA, the right is a YUV
    	virtual void 	processRightYUV(imageStruct &image, imageStruct &right); 
};

#endif	// for header file
