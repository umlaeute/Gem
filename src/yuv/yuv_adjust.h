/*
 *  yuv_adjust.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_YUV_ADJUST_H_ 
#define INCLUDE_YUV_ADJUST_H_ 

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_adjust
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   adjustment of the Y U V channels
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_adjust : public GemPixObj
{
CPPEXTERN_HEADER(yuv_adjust, GemPixObj)

    public:

	    //////////
	    // Constructor
    	yuv_adjust();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_adjust();

    	//////////
    	// Do the processing
    	virtual void 	processImage(imageStruct &image);
    	
    	virtual void 	processYUVImage(imageStruct &image);
        
        void	    	yMess(float size);
        void	    	uMess(float size);
        void	    	vMess(float size);
    	
        short	Y,U,V;
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

