/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Add two images together.

    Copyright (c) 1997-1998 Mark Danks. mark@danks.org
    Copyright (c) G¸nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f¸r::uml‰ute. IEM. zmoelnig@iem.kug.ac.at
    Copyright (c) 2002 James Tittle & Chris Clepper
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_MIX_H_
#define INCLUDE_PIX_MIX_H_

#include "Base/GemPixDualObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_mix
    
    Blends two images together.

KEYWORDS
    pix
    
DESCRIPTION

    gain $1 $2 - sets the gain for the left and right images.

-----------------------------------------------------------------*/
class GEM_EXTERN pix_mix : public GemPixDualObj
{
    CPPEXTERN_HEADER(pix_mix, GemPixDualObj)

    public:

	    //////////
    	// Constructor
    	pix_mix();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_mix();

    	//////////
    	// Do the processing
    	virtual void 	processRGBA_RGBA(imageStruct &image, imageStruct &right);
        
    	//////////
    	// Do the processing
    	virtual void 	processYUV_YUV(imageStruct &image, imageStruct &right);
        
        virtual void 	processYUVAltivec (imageStruct &image, imageStruct &right);
        
        virtual void 	gainMess (float X, float Y);
        
        long imageGain,rightGain;
        
         
         private:
    
    	//////////
    	// Static member functions
    	
        static void gainCallback       (void *data, t_floatarg X, t_floatarg Y);
         
         

};

#endif	// for header file


