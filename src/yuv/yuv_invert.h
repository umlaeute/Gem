/*
 *  yuv_invert.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_yuv_invert_H_ 
#define INCLUDE_yuv_invert_H_ 

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_invert
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   changes a color image to black and white by setting the luma to 0
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_invert : public GemPixObj
{
CPPEXTERN_HEADER(yuv_invert, GemPixObj)

    public:

	    //////////
	    // Constructor
    	yuv_invert();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_invert();

    	//////////
    	// Do the processing
    	virtual void 	processImage(imageStruct &image);
    	
    	virtual void 	processYUVImage(imageStruct &image);

    private:
    
    	//////////
    	// Static member functions


};

#endif

