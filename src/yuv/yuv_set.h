/*
 *  yuv_set.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_YUV_SET_H_ 
#define INCLUDE_YUV_SET_H_ 

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    yuv_set
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   setment of the Y U V channels
   
-----------------------------------------------------------------*/

class GEM_EXTERN yuv_set : public GemPixObj
{
CPPEXTERN_HEADER(yuv_set, GemPixObj)

    public:

	    //////////
	    // Constructor
    	yuv_set();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~yuv_set();

    	//////////
    	// Do the processing
    	virtual void 	processImage(imageStruct &image);
    	
    	virtual void 	processYUVImage(imageStruct &image);
        
        void	    	yMess(float size);
        void	    	uMess(float size);
        void	    	vMess(float size);
    	
        unsigned char	Y,U,V;
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

