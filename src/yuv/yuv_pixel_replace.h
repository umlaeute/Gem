/*
 *  yuv_pixel_replace.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_yuv_pixel_replace_H_ 
#define INCLUDE_yuv_pixel_replace_H_ 

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_pixel_replace
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   template for yuv_ objects
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_pixel_replace : public GemPixObj
{
CPPEXTERN_HEADER(yuv_pixel_replace, GemPixObj)

    public:

	    //////////
	    // Constructor
    	yuv_pixel_replace();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_pixel_replace();

    	//////////
    	// Do the processing
    	virtual void 	processImage(imageStruct &image);
    	
        //////////
    	// Do the YUV processing
    	virtual void 	processYUVImage(imageStruct &image);
        
        unsigned char m_Yrange,m_Urange,m_Vrange,m_Yvalue,m_Uvalue,m_Vvalue;
        unsigned char m_Yreplace,m_Ureplace,m_Vreplace;
        
    private:
    
    	//////////
    	// Static member functions
        static void rangeCallback       (void *data, t_floatarg Yval, t_floatarg Uval,t_floatarg Vval);
        static void valueCallback       (void *data, t_floatarg Yval, t_floatarg Uval,t_floatarg Vval);
    	static void replaceCallback       (void *data, t_floatarg Yval, t_floatarg Uval,t_floatarg Vval);


};

#endif

