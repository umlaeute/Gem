/*
 *  pix_posterize.h
 *  gem_darwin
 *
 *  Created by James Tittle on Fri Oct 18 2002.
 *  Copyleft (l) 2002 tigital. All rights reserved.
 *
 */

#ifndef INCLUDE_PIX_POSTERIZE_H_ 
#define INCLUDE_PIX_POSTERIZE_H_ 

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_posterize

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   adjustment of the Y U V channels
   
-----------------------------------------------------------------*/

class GEM_EXTERN pix_posterize : public GemPixObj
{
CPPEXTERN_HEADER(pix_posterize, GemPixObj)

    public:

	    //////////
	    // Constructor
    	pix_posterize();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_posterize();

    	//////////
    	// Do the processing
    	virtual void 	processImage(imageStruct &image);
    	
    	virtual void 	processYUVImage(imageStruct &image);
        
        void	    	factorMess(float f);
        void	    	limitMess(float l);
    	
        t_inlet         *inletF;
        t_inlet         *inletL;
        int		factor;
        int		limit;
        
    private:
    
    	//////////
    	// Static member functions
    	static void 	factorMessCallback(void *data, t_floatarg size);
        static void 	limitMessCallback(void *data, t_floatarg size);

};

#endif

