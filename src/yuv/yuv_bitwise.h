/*
 *  yuv_bitwise.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_yuv_bitwise_H_ 
#define INCLUDE_yuv_bitwise_H_ 

#include "yuv_dual.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_bitwise
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   template for yuv_ objects
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_bitwise : public yuv_dual
{
CPPEXTERN_HEADER(yuv_bitwise, yuv_dual)

    public:

	    //////////
	    // Constructor
    	yuv_bitwise();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_bitwise();

    	//////////
    	// Do the processing
    	virtual void 	processDualImage(imageStruct &image, imageStruct &right);
    	
        //////////
    	// Do the YUV processing
    	virtual void 	processDualYUV(imageStruct &image, imageStruct &right);
        
        virtual void 	processRightYUV(imageStruct &image, imageStruct &right);
        
        virtual void 	processLeftYUV(imageStruct &image, imageStruct &right);
        
        int 		m_mode;
        
    private:
    
    	//////////
    	// Static member functions
    	
        static void modeCallback       (void *data, t_floatarg state);

};

#endif

