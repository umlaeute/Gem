/*
 *  yuv_compare.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_YUV_COMPARE_H_ 
#define INCLUDE_YUV_COMPARE_H_ 

#include "yuv_dual.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_compare
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   template for yuv_ objects
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_compare : public yuv_dual
{
CPPEXTERN_HEADER(yuv_compare, yuv_dual)

    public:

	    //////////
	    // Constructor
    	yuv_compare();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_compare();

    	//////////
    	// Do the processing
    	virtual void 	processDualImage(imageStruct &image, imageStruct &right);
    	
        //////////
    	// Do the YUV processing
    	virtual void 	processDualYUV(imageStruct &image, imageStruct &right);
        
        virtual void 	processRightYUV(imageStruct &image, imageStruct &right);
        
        virtual void 	processLeftYUV(imageStruct &image, imageStruct &right);
        

        
        int m_direction;
        
    private:
    
    	//////////
    	// Static member functions
    	static void directionCallback       (void *data, t_floatarg state);


};

#endif

