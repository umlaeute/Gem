/*
 *  yuv_blur.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_yuv_blur_H_ 
#define INCLUDE_yuv_blur_H_ 

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_blur
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   template for yuv_ objects
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_blur : public GemPixObj
{
CPPEXTERN_HEADER(yuv_blur, GemPixObj)

    public:

	    //////////
	    // Constructor
    	yuv_blur();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_blur();

    	//////////
    	// Do the processing
    	virtual void 	processImage(imageStruct &image);
    	
        //////////
    	// Do the YUV processing
    	virtual void 	processYUVImage(imageStruct &image);
        
        unsigned char  *saved;
        int		m_blur;
        int		m_blurH,m_blurW,m_blurSize,m_blurBpp;
        t_inlet         *inletBlur;

        
    private:
    
    	//////////
    	// Static member functions
    	
        static void blurCallback       (void *data, t_floatarg value);


};

#endif

