/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Change the overall gain of a pix

    Copyright (c) 1997-1998 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    Copyright (c) 2002 James Tittle & Chris Clepper
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_GAIN_H_
#define INCLUDE_PIX_GAIN_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_gain
    
    Change the overall gain of a pix

KEYWORDS
    pix
    
DESCRIPTION

    Inlet for a list - "vec_gain"
    Inlet for a float - "ft1"

    "vec_gain" - The gain vector to set to
    "ft1" - The gain amount (all gains are equal)
   
-----------------------------------------------------------------*/
class GEM_EXTERN pix_gain : public GemPixObj
{
    CPPEXTERN_HEADER(pix_gain, GemPixObj)

    public:

	    //////////
	    // Constructor
    	pix_gain();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_gain();

    	//////////
    	// Do the processing
    	virtual void 	processRGBAImage(imageStruct &image);
    	virtual void 	processGrayImage(imageStruct &image);
    	virtual void 	processYUVImage(imageStruct &image);
    	 //////////
    	// altivec
    	virtual void 	processYUV_Altivec(imageStruct &image);   	
    	//////////
    	// Set the new gain
    	void	    	vecGainMess(int argc, t_atom *argv);
    	
    	//////////
    	// Set the new gain
    	void	    	floatGainMess(float gain);
    	 
    	//////////
    	// The new gain
    	float		  	m_gain[4];
    
    private:
    
    	//////////
    	// Static member functions
    	static void 	vecGainMessCallback(void *data, t_symbol *, int argc, t_atom *argv);
    	static void 	floatGainMessCallback(void *data, t_floatarg gain);
};

#endif	// for header file
