/*
 *  yuv_mix.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_yuv_mix_H_ 
#define INCLUDE_yuv_mix_H_ 

#include "yuv_dual.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_mix
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   template for yuv_ objects
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_mix : public yuv_dual
{
CPPEXTERN_HEADER(yuv_mix, yuv_dual)

    public:

	    //////////
	    // Constructor
    	yuv_mix();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_mix();

    	//////////
    	// Do the processing
    	virtual void 	processDualImage(imageStruct &image, imageStruct &right);
    	
        //////////
    	// Do the YUV processing
    	virtual void 	processDualYUV(imageStruct &image, imageStruct &right);
        
        virtual void 	processRightYUV(imageStruct &image, imageStruct &right);
        
        virtual void 	processLeftYUV(imageStruct &image, imageStruct &right);
        
        long imageGain,rightGain;
        
    private:
    
    	//////////
    	// Static member functions
    	
        static void gainCallback       (void *data, t_floatarg X, t_floatarg Y);
        static void botCallback       (void *data, t_floatarg X, t_floatarg Y);
        static void modeCallback       (void *data, t_floatarg state);

};

#endif

