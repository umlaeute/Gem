/*
 *  yuv_bandw.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_YUV_BANDW_H_ 
#define INCLUDE_YUV_BANDW_H_ 

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_bandw
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   changes a color image to black and white by setting the luma to 0
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_bandw : public GemPixObj
{
CPPEXTERN_HEADER(yuv_bandw, GemPixObj)

    public:

	    //////////
	    // Constructor
    	yuv_bandw();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_bandw();

    	//////////
    	// Do the processing
    	virtual void 	processImage(imageStruct &image);
    	
    	virtual void 	processYUVImage(imageStruct &image);
        
        void	    	yMess(float size);
        void	    	uMess(float size);
        void	    	vMess(float size);
    	
        char	Y,U,V;
        t_inlet         *inletY;
        t_inlet         *inletU;
        t_inlet         *inletV;
        
    private:
    
    	//////////
    	// Static member functions
    	static void 	yMessCallback(void *data, t_floatarg size);
        static void 	uMessCallback(void *data, t_floatarg size);
        static void 	vMessCallback(void *data, t_floatarg size);


};

#endif

