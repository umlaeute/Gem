/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    add offset-values to the RGBA-Channels (without clamping to 0..255)

    Copyright (c) 1997-1998 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    Copyright (c) 2002 James Tittle & Chris Clepper
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_OFFSET_H_
#define INCLUDE_PIX_OFFSET_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_offset
    
    Change the overall gain of a pix

KEYWORDS
    pix
    
DESCRIPTION

    Inlet for a list - "vec_offset"
    Inlet for a float - "ft1"

    "vec_offset" - The offset vector to set to
    "ft1" - The offset amount (all offsets are equal)
   
-----------------------------------------------------------------*/
class GEM_EXTERN pix_offset : public GemPixObj
{
    CPPEXTERN_HEADER(pix_offset, GemPixObj)

    public:

	    //////////
	    // Constructor
    	pix_offset();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_offset();

    	//////////
    	// Do the processing
    	virtual void 	processRGBAImage(imageStruct &image);
    	virtual void 	processGrayImage(imageStruct &image);
    	virtual void 	processYUVImage(imageStruct &image);
#ifdef __MMX__
	//////////
	// MMX
    	virtual void 	processRGBAMMX(imageStruct &image); 
     	virtual void 	processYUVMMX(imageStruct &image); 
   	virtual void 	processGrayMMX(imageStruct &image); 
#endif
#ifdef __VEC__
	//////////
    	// altivec
    	virtual void 	processYUVAltivec(imageStruct &image); 
#endif

    	//////////
    	// Set the new offset
    	void	    	vecOffsetMess(int argc, t_atom *argv);
    	
    	//////////
    	// Set the new offset
    	void	    	floatOffsetMess(float foffset);
    	
    	//////////
    	// The new offset
    	unsigned char 	m_offset[4];
        short Y,U,V;

    private:
    
    	//////////
    	// Static member functions
    	static void 	vecOffsetMessCallback(void *data, t_symbol *, int argc, t_atom *argv);
    	static void 	floatOffsetMessCallback(void *data, t_floatarg offset);
};

#endif	// for header file
