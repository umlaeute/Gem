/*
 *  yuv_difference.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_yuv_difference_H_ 
#define INCLUDE_yuv_difference_H_ 

#include "yuv_dual.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_difference
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   subracts two images
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_difference : public yuv_dual
{
CPPEXTERN_HEADER(yuv_difference, yuv_dual)

    public:

	    //////////
	    // Constructor
    	yuv_difference();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_difference();

    	//////////
    	// Do the processing
    	virtual void 	processDualImage(imageStruct &image, imageStruct &right);
    	
        //////////
    	// Do the YUV processing
    	virtual void 	processDualYUV(imageStruct &image, imageStruct &right);
        
        virtual void 	processRightYUV(imageStruct &image, imageStruct &right);
        
        virtual void 	processLeftYUV(imageStruct &image, imageStruct &right);
        
    private:
    
    	//////////
    	// Static member functions
    	


};

#endif

