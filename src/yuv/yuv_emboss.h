/*
 *  yuv_emboss.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_YUV_EMBOSS_H_ 
#define INCLUDE_YUV_EMBOSS_H_ 

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_emboss
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   template for yuv_ objects
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_emboss : public GemPixObj
{
CPPEXTERN_HEADER(yuv_emboss, GemPixObj)

    public:

	    //////////
	    // Constructor
    	yuv_emboss();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_emboss();

        //////////
    	// Do the YUV processing
    	virtual void 	processYUVImage(imageStruct &image);
        
        
        
    private:
    
    	//////////
    	// Static member functions
    	


};

#endif

