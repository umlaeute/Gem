/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Change pix to greyscale

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    Copyright (c) 2002 James Tittle & Chris Clepper
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_2GREY_H_
#define INCLUDE_PIX_2GREY_H_

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
class GEM_EXTERN pix_2grey : public GemPixObj
{
    CPPEXTERN_HEADER(pix_2grey, GemPixObj)

    public:

	    //////////
	    // Constructor
    	pix_2grey();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_2grey();

    	//////////
    	// Do the processing
    	virtual void 	processRGBAImage(imageStruct &image);
	virtual void 	processYUVImage(imageStruct &image);
#ifdef __MMX__
# ifndef __APPLE__
        virtual void 	processRGBAMMX(imageStruct &image);
# endif
        virtual void 	processYUVMMX(imageStruct &image);
#endif
#ifdef __SSE2__
        virtual void 	processYUVSSE2(imageStruct &image);
#endif
#ifdef __VEC__
        virtual void 	processRGBAAltivec(imageStruct &image);
        virtual void 	processYUVAltivec(imageStruct &image);
#endif
     	    	  
    	//////////
    	// Do the processing - this is a no-op
    	virtual void 	processGrayImage(imageStruct &image) { }
};

#endif	// for header file
