/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Flip a pix

    Copyright (c) 1997-1998 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_FLIP_H_
#define INCLUDE_PIX_FLIP_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_flip
    
    Flip a pix

KEYWORDS
    pix
    
DESCRIPTION

    "horizontal" - Flip horizontally
    "vertical" - Flip vertically
    "both" - Flip in both directiosn
    "none" - Do nothing
   
-----------------------------------------------------------------*/
class GEM_EXTERN pix_flip : public GemPixObj
{
    CPPEXTERN_HEADER(pix_flip, GemPixObj)

    public:

	    //////////
	    // Constructor
    	pix_flip();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_flip();

    	//////////
    	// Do the processing
    	virtual void 	processImage(imageStruct &image);

    	//////////
    	enum FlipType	{ HOR, VERT, BOTH, NONE };
    	
    	//////////
    	void	    	flipMess(FlipType type);

    	//////////
    	// The flip type
    	FlipType 	    m_flip;
    
    private:
    
    	//////////
    	// Static member functions
    	static void 	horMessCallback(void *data);
    	static void 	vertMessCallback(void *data);
    	static void 	bothMessCallback(void *data);
    	static void 	noneMessCallback(void *data);
};

#endif	// for header file
