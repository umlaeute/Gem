/*
 *  yuv_clamp.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_YUV_CLAMP_H_ 
#define INCLUDE_YUV_CLAMP_H_ 

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_clamp
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   template for yuv_ objects
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_clamp : public GemPixObj
{
CPPEXTERN_HEADER(yuv_clamp, GemPixObj)

    public:

	    //////////
	    // Constructor
    	yuv_clamp();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_clamp();

    	//////////
    	// Do the processing
    	virtual void 	processImage(imageStruct &image);
    	
        //////////
    	// Do the YUV processing
    	virtual void 	processYUVImage(imageStruct &image);
        
        unsigned char Yhi,Ylo,Uhi,Ulo,Vhi,Vlo;
        
    private:
    
    	//////////
    	// Static member functions
    	static void YClampCallback       (void *data, t_floatarg Hival, t_floatarg Loval);
        static void UClampCallback       (void *data, t_floatarg Hival, t_floatarg Loval);
        static void VClampCallback       (void *data, t_floatarg Hival, t_floatarg Loval);

};

#endif

