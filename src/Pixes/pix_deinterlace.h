/*
 *  pix_deinterlace.h
 *  GEM_darwin
 *
 *  Created by lincoln on 11/18/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef INCLUDE_PIX_DEINTERLACE_H_
#define INCLUDE_PIX_DEINTERLACE_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_2grey
    
    Change pix to greyscale

KEYWORDS
    pix
    
DESCRIPTION
   
-----------------------------------------------------------------*/
class GEM_EXTERN pix_deinterlace : public GemPixObj
{
    CPPEXTERN_HEADER(pix_deinterlace, GemPixObj)

    public:

	    //////////
	    // Constructor
    	pix_deinterlace();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_deinterlace();

    	//////////
    	// Do the processing
    //	virtual void 	processRGBAImage(imageStruct &image);
		virtual void 	processYUVImage(imageStruct &image);
#ifdef __MMX__
 //       virtual void 	processRGBAMMX(imageStruct &image);
 //       virtual void 	processYUVMMX(imageStruct &image);
#endif
#ifdef __VEC__
    //    virtual void 	processRGBAAltivec(imageStruct &image);
        virtual void 	processYUVAltivec(imageStruct &image);
#endif
     	    	  
    	//////////
    	// Do the processing - this is a no-op
  //  	virtual void 	processGrayImage(imageStruct &image) { }
		
		int	m_mode;
		
	private:
	
		static void 	modeMessCallback(void *data, t_floatarg contrast);
};



#endif	// for header file
