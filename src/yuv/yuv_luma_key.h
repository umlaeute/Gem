
/*
 *  yuv_luma_key.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_yuv_luma_key_H_ 
#define INCLUDE_yuv_luma_key_H_ 

#include "yuv_dual.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_luma_key
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   chroma keying
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_luma_key : public yuv_dual
{
CPPEXTERN_HEADER(yuv_luma_key, yuv_dual)

    public:

	    //////////
	    // Constructor
    	yuv_luma_key();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_luma_key();

    	//////////
    	// Do the processing
    	virtual void 	processDualImage(imageStruct &image, imageStruct &right);
    	
        //////////
    	// Do the YUV processing
    	virtual void 	processDualYUV(imageStruct &image, imageStruct &right);
        
        virtual void 	processRightYUV(imageStruct &image, imageStruct &right);
        
        virtual void 	processLeftYUV(imageStruct &image, imageStruct &right);
        

        
        int m_direction;
        unsigned char m_Yrange,m_Urange,m_Vrange,m_Yvalue,m_Uvalue,m_Vvalue;
        
    private:
    
    	//////////
    	// Static member functions
    	static void directionCallback       (void *data, t_floatarg state);
        static void rangeCallback       (void *data, t_floatarg Yval, t_floatarg Uval,t_floatarg Vval);
        static void valueCallback       (void *data, t_floatarg Yval, t_floatarg Uval,t_floatarg Vval);

};

#endif

