/*
 *  yuv_pixel_data.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_YUV_PIXEL_DATA_H_ 
#define INCLUDE_YUV_PIXEL_DATA_H_ 

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_pixel_data
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   template for yuv_ objects
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_pixel_data : public GemPixObj
{
CPPEXTERN_HEADER(yuv_pixel_data, GemPixObj)

    public:

	    //////////
	    // Constructor
    	yuv_pixel_data();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_pixel_data();

    	//////////
    	// Do the processing
    	virtual void 	processImage(imageStruct &image);
    	
        //////////
    	// Do the YUV processing
    	virtual void 	processYUVImage(imageStruct &image);
        
        long m_height, m_width;
        unsigned char YVal,UVal,VVal;
        t_outlet *m_YOut,*m_UOut,*m_VOut;
        
    private:
    
    	//////////
    	// Static member functions
    	static void locationCallback       (void *data, t_floatarg Hval, t_floatarg Wval);


};

#endif

