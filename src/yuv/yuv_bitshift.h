/*
 *  yuv_bitshift.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_yuv_bitshift_H_ 
#define INCLUDE_yuv_bitshift_H_ 

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_bitshift
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   changes a color image to black and white by setting the luma to 0
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_bitshift : public GemPixObj
{
CPPEXTERN_HEADER(yuv_bitshift, GemPixObj)

    public:

	    //////////
	    // Constructor
    	yuv_bitshift();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_bitshift();

    	//////////
    	// Do the processing
    	virtual void 	processImage(imageStruct &image);
    	
    	virtual void 	processYUVImage(imageStruct &image);

        unsigned char	Y,U,V;

    private:
    
    	//////////
    	// Static member functions
    	static void 	yMessCallback(void *data, t_floatarg size);
        static void 	uMessCallback(void *data, t_floatarg size);
        static void 	vMessCallback(void *data, t_floatarg size);

};

#endif

