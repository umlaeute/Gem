/*
 *  yuv_blank.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_YUV_BLANK_H_ 
#define INCLUDE_YUV_BLANK_H_ 

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_blank
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   template for yuv_ objects
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_blank : public GemPixObj
{
CPPEXTERN_HEADER(yuv_blank, GemPixObj)

    public:

	    //////////
	    // Constructor
    	yuv_blank();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_blank();

    	//////////
    	// Do the processing
    	virtual void 	processImage(imageStruct &image);
    	
        //////////
    	// Do the YUV processing
    	virtual void 	processYUVImage(imageStruct &image);
        
        
        
    private:
    
    	//////////
    	// Static member functions
    	


};

#endif

