/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia


    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) G¸nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f¸r::uml‰ute. IEM. zmoelnig@iem.kug.ac.at
    Copyright (c) 2003 Daniel Heckenberg

    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_pix_compare_H_ 
#define INCLUDE_pix_compare_H_ 

#include "Base/GemPixDualObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_compare
    
    

KEYWORDS
    pix
    yuv
    
DESCRIPTION

   compares the pixel values of two images
   
-----------------------------------------------------------------*/

class GEM_EXTERN pix_compare : public GemPixDualObj
{
CPPEXTERN_HEADER(pix_compare, GemPixDualObj)

    public:

	    //////////
	    // Constructor
    	pix_compare();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_compare();

    	
    	//////////
    	// Do the processing
    	virtual void 	processRGBA_RGBA(imageStruct &image, imageStruct &right);
        
    	//////////
    	// Do the processing
    	virtual void 	processYUV_YUV(imageStruct &image, imageStruct &right);

        //////////
    	// Do the Altivec processing
    	virtual void 	processYUVAltivec(imageStruct &image, imageStruct &right);

        
        int m_direction;
        
    private:
    
    	//////////
    	// Static member functions
    	static void directionCallback       (void *data, t_floatarg state);


};

#endif

