/*
 *  yuv_add.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_YUV_ADD_H_ 
#define INCLUDE_YUV_ADD_H_ 

#include "yuv_dual.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_add
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   template for yuv_ objects
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_add : public yuv_dual
{
CPPEXTERN_HEADER(yuv_add, yuv_dual)

    public:

	    //////////
	    // Constructor
    	yuv_add();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_add();

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

