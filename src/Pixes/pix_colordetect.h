#ifndef _INCLUDE__GEM_PIXES_PIX_COLOR_DETECT_H_
#define _INCLUDE__GEM_PIXES_PIX_COLOR_DETECT_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_colordetect
    
    Detects red, green, blue, yellow, white, black in an image

KEYWORDS
    pix, color
    
DESCRIPTION

-----------------------------------------------------------------*/
class pix_colordetect : public GemPixObj
{
    CPPEXTERN_HEADER(pix_colordetect, GemPixObj);

    public:

	    //////////
	    // Constructor
    	pix_colordetect();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_colordetect();

    	//////////
    	// Do the processing
    	virtual void 	processRGBAImage(imageStruct &image);
      // virtual void 	processGrayImage(imageStruct &image);
    	// virtual void 	processYUVImage(imageStruct &image);
	
    private:
};

#endif	// for header file
