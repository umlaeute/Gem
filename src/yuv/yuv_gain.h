/*
 *  yuv_gain.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_YUV_gain_H_ 
#define INCLUDE_YUV_gain_H_ 

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_gain
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   template for yuv_ objects
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_gain : public GemPixObj
{
CPPEXTERN_HEADER(yuv_gain, GemPixObj)

    public:

	    //////////
	    // Constructor
    	yuv_gain();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_gain();

    	//////////
    	// Do the processing
    	virtual void 	processImage(imageStruct &image);
    	
        //////////
    	// Do the YUV processing
    	virtual void 	processYUVImage(imageStruct &image);
        
        void	    	gainMess(float gain);
        float	G;
        t_inlet         *inletGain;
        
    private:
    
    	//////////
    	// Static member functions
    	static void 	gainMessCallback(void *data, t_floatarg gain);


};

#endif

