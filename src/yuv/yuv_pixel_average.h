/*
 *  yuv_pixel_average.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_yuv_pixel_average_H_ 
#define INCLUDE_yuv_pixel_average_H_ 

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_pixel_average
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   template for yuv_ objects
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_pixel_average : public GemPixObj
{
CPPEXTERN_HEADER(yuv_pixel_average, GemPixObj)

    public:

	    //////////
	    // Constructor
    	yuv_pixel_average();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_pixel_average();

    	//////////
    	// Do the processing
    	virtual void 	processImage(imageStruct &image);
    	
        //////////
    	// Do the YUV processing
    	virtual void 	processYUVImage(imageStruct &image);
        
        int topX,topY,lowX,lowY;
        int dimenX,dimenY;
        long m_height, m_width;
        unsigned char YVal,UVal,VVal;
        t_outlet *m_YOut,*m_UOut,*m_VOut;
        
    private:
    
    	//////////
    	// Static member functions
        static void topCallback       (void *data, t_floatarg X, t_floatarg Y);
        static void lowerCallback       (void *data, t_floatarg X, t_floatarg Y);
        static void dimenCallback       (void *data, t_floatarg X, t_floatarg Y);
        static void modeCallback       (void *data, t_floatarg state);


};

#endif

