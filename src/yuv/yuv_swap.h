/*
 *  yuv_swap.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_yuv_swap_H_ 
#define INCLUDE_yuv_swap_H_ 

#include "yuv_dual.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_swap
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   subracts two images
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_swap : public yuv_dual
{
CPPEXTERN_HEADER(yuv_swap, yuv_dual)

    public:

	    //////////
	    // Constructor
    	yuv_swap();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_swap();

    	//////////
    	// Do the processing
    	virtual void 	processDualImage(imageStruct &image, imageStruct &right);
    	
        //////////
    	// Do the YUV processing
    	virtual void 	processDualYUV(imageStruct &image, imageStruct &right);
        
        virtual void 	processRightYUV(imageStruct &image, imageStruct &right);
        
        virtual void 	processLeftYUV(imageStruct &image, imageStruct &right);
        

        
        unsigned char	Y,U,V;
        
    private:
    
    	//////////
    	// Static member functions
    	static void 	yMessCallback(void *data, t_floatarg size);
        static void 	uMessCallback(void *data, t_floatarg size);
        static void 	vMessCallback(void *data, t_floatarg size);


};

#endif

