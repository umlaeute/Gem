/*
 *  yuv_split.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_yuv_split_H_ 
#define INCLUDE_yuv_split_H_ 

#include "yuv_dual.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_split
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   template for yuv_ objects
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_split : public yuv_dual
{
CPPEXTERN_HEADER(yuv_split, yuv_dual)

    public:

	    //////////
	    // Constructor
    	yuv_split();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_split();

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

