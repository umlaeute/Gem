/*
 *  yuv_mask.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_yuv_mask_H_ 
#define INCLUDE_yuv_mask_H_ 

#include "yuv_dual.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_mask
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   template for yuv_ objects
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_mask : public yuv_dual
{
CPPEXTERN_HEADER(yuv_mask, yuv_dual)

    public:

	    //////////
	    // Constructor
    	yuv_mask();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_mask();

    	//////////
    	// Do the processing
    	virtual void 	processDualImage(imageStruct &image, imageStruct &right);
    	
        //////////
    	// Do the YUV processing
    	virtual void 	processDualYUV(imageStruct &image, imageStruct &right);
        
        virtual void 	processRightYUV(imageStruct &image, imageStruct &right);
        
        virtual void 	processLeftYUV(imageStruct &image, imageStruct &right);
        

        
        int topX,topY,lowX,lowY;
        int dimenX,dimenY;
        
    private:
    
    	//////////
    	// Static member functions
    	
        static void topCallback       (void *data, t_floatarg X, t_floatarg Y);
        static void lowerCallback       (void *data, t_floatarg X, t_floatarg Y);
        static void dimenCallback       (void *data, t_floatarg X, t_floatarg Y);
        static void modeCallback       (void *data, t_floatarg state);

};

#endif

