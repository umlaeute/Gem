/*
 *  pix_blur.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_pix_blur_H_ 
#define INCLUDE_pix_blur_H_ 

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_blur
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   template for yuv_ objects
   
-----------------------------------------------------------------*/

class GEM_EXTERN pix_blur : public GemPixObj
{
CPPEXTERN_HEADER(pix_blur, GemPixObj)

    public:

	    //////////
	    // Constructor
    	pix_blur();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_blur();

    	//////////
    	// Do the processing
    	virtual void 	processRGBAImage(imageStruct &image);
    	
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

