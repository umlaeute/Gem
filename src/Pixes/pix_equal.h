/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

Clamp pixel values to a threshold

Copyright (c) 1997-1998 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
Copyright (c) 2002 James Tittle & Chris Clepper
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_EQUAL_H_
#define _INCLUDE__GEM_PIXES_PIX_EQUAL_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_equal
    
    mark pixels nearly equal to a given color, within given bounds

KEYWORDS
    pix
    
DESCRIPTION

    Inlet for a list - "vec_thresh"
    Inlet for a list - "vec_low"
    Inlet for a float  - "vec_up"
    
    "vec_color" - The color vector to search for
    "vec_low" - Lower bound to search for
    "vec_up" - Upper bound to search for
   
-----------------------------------------------------------------*/
class GEM_EXTERN pix_equal : public GemPixObj
{
    CPPEXTERN_HEADER(pix_equal, GemPixObj);

    public:

        //////////
        // Constructor
    	pix_equal();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_equal();

    	//////////
    	// Do the processing
    	virtual void 	processRGBAImage(imageStruct &image);
    	
    	//////////
    	// Do the processing
//      virtual void 	processGrayImage(imageStruct &image);
        	
        //////////
    	// Do the processing
//      virtual void 	processYUVImage(imageStruct &image);
		
//#ifdef __VEC__
	//////////
    	// Do the processing
//      virtual void 	processYUVAltivec(imageStruct &image);
//#endif  
    	//////////
    	// Set the new color vector
    	void	    	vecLowerBoundMess(int argc, t_atom *argv);
    	void	    	vecUpperBoundMess(int argc, t_atom *argv);

    	//////////
    	// The bounds
    	unsigned char  	m_upper[4];
    	unsigned char  	m_lower[4];

    private:
    
    	//////////
    	// Static member functions
    	static void 	vecLowerMessCallback(void *data, t_symbol *, int argc, t_atom *argv);
    	static void 	vecUpperMessCallback(void *data, t_symbol *, int argc, t_atom *argv);
};

#endif	// for header file
