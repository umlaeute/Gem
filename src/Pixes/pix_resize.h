/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Resizes an image

    Copyright (c) 1997-1998 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_RESIZE_H_
#define INCLUDE_PIX_RESIZE_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_resize
    
    Resizes an image

KEYWORDS
    pix

-----------------------------------------------------------------*/
class GEM_EXTERN pix_resize : public GemPixObj
{
    CPPEXTERN_HEADER(pix_resize, GemPixObj)

    public:

	    //////////
	    // Constructor
    	pix_resize();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_resize();

    	//////////
    	// Do the processing
    	virtual void 	processImage(imageStruct &image);
};

#endif	// for header file
