#ifndef _INCLUDE__GEM_PIXES_PIX_COLOR_DETECT_H_
#define _INCLUDE__GEM_PIXES_PIX_COLOR_DETECT_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_colorclassify
    
    Detects red, green, blue, yellow, white, black in an image

KEYWORDS
    pix, color
    
DESCRIPTION

-----------------------------------------------------------------*/
class pix_colorclassify : public GemPixObj
{
    CPPEXTERN_HEADER(pix_colorclassify, GemPixObj);

    public:

	    //////////
	    // Constructor
    	pix_colorclassify();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_colorclassify();

    	//////////
    	// Do the processing
    	virtual void 	processRGBAImage(imageStruct &image);
      // virtual void 	processGrayImage(imageStruct &image);
    	// virtual void 	processYUVImage(imageStruct &image);
	
    private:
};

#endif	// for header file
