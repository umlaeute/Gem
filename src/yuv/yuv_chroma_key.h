
/*
 *  yuv_chroma_key.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_YUV_CHROMA_KEY_H_ 
#define INCLUDE_YUV_CHROMA_KEY_H_ 

//#include "yuv_dual.h"
#include "Base/GemPixDualObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_chroma_key
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   chroma keying
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_chroma_key : public GemPixDualObj //yuv_dual
{
  CPPEXTERN_HEADER(yuv_chroma_key, GemPixDualObj)//yuv_dual)

    public:

	    //////////
	    // Constructor
    	yuv_chroma_key();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_chroma_key();

        //////////
    	// Do the YUV processing
    	virtual void 	processYUV_YUV(imageStruct &image, imageStruct &right);
        
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

