/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Change the overall color of a pix image

    Copyright (c) 1997-1998 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    Copyright (c) 2002 James Tittle & Chris Clepper
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_COLOR_H_
#define INCLUDE_PIX_COLOR_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_color
    
    Change the overall color of a pix image

KEYWORDS
    pix
    
DESCRIPTION

    Inlet for a list - "vec_gain"

    "vec_gain" - The color vector to set to
   
-----------------------------------------------------------------*/
class pix_color : public GemPixObj
{
    CPPEXTERN_HEADER(pix_color, GemPixObj)

    public:

	    //////////
	    // Constructor
    	pix_color();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_color();

    	//////////
    	// Do the processing
    	virtual void 	processImage(imageStruct &image);
        
	//////////
	virtual void 	processYUVImage(imageStruct &image);
    	
    	//////////
    	// Set the new color
    	void	    	vecGainMess(float red, float green, float blue);
    	
    	//////////
    	// The new color
    	unsigned char  	m_color[3];
    
    private:
    
    	//////////
    	// Static member functions
    	static void 	vecGainMessCallback(void *data, t_floatarg red, t_floatarg green, t_floatarg blue);
};

#endif	// for header file
