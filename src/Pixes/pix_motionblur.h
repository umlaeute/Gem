/*
 *  pix_motionblur.h
 *  gem_darwin
 *
 *  Created by chris clepper on Mon Oct 07 2002.
 *  Copyright (c) 2002 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INCLUDE_PIX_MOTIONBLUR_H_ 
#define INCLUDE_PIX_MOTIONBLUR_H_ 

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_motionblur
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

  does motion blur by mixing the current and previous frames for a video 'feedback' effect
   
-----------------------------------------------------------------*/

class GEM_EXTERN pix_motionblur : public GemPixObj
{
CPPEXTERN_HEADER(pix_motionblur, GemPixObj)

    public:

	    //////////
	    // Constructor
    	pix_motionblur();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_motionblur();

    	//////////
    	// Do the processing
    	virtual void 	processRGBAImage(imageStruct &image);
      	virtual void 	processGrayImage(imageStruct &image);
  	virtual void 	processYUVImage(imageStruct &image);
        virtual void 	processYUVAltivec(imageStruct &image);
        
         int  *saved;
        float		m_motionblur;
        int		m_motionblurH,m_motionblurW,m_motionblurSize,m_motionblurBpp;
        t_inlet         *inletmotionblur;

        
    private:
    
    	//////////
    	// Static member functions
    	
        static void motionblurCallback       (void *data, t_floatarg value);


};

#endif

