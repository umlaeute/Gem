/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    add offset-values to the RGBA-Channels (without clamping to 0..255)

    Copyright (c) 1997-1998 Mark Danks. mark@danks.org
    Copyright (c) G¸nther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f¸r::uml‰ute. IEM. zmoelnig@iem.kug.ac.at
    Copyright (c) 2002 James Tittle & Chris Clepper
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_pix_duotone_H_
#define INCLUDE_pix_duotone_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_duotone
    
    Change the overall gain of a pix

KEYWORDS
    pix
    
DESCRIPTION

    Inlet for a list - "vec_offset"
    Inlet for a float - "ft1"

    "vec_offset" - The offset vector to set to
    "ft1" - The offset amount (all offsets are equal)
   
-----------------------------------------------------------------*/
class GEM_EXTERN pix_duotone : public GemPixObj
{
    CPPEXTERN_HEADER(pix_duotone, GemPixObj)

    public:

	    //////////
	    // Constructor
    	pix_duotone();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_duotone();

    	//////////
    	// Do the processing
    	virtual void 	processRGBAImage(imageStruct &image);
    	virtual void 	processRGBImage(imageStruct &image);
		virtual void 	processGrayImage(imageStruct &image);
    	virtual void 	processYUVImage(imageStruct &image);
         //////////
    	// altivec
    	//virtual void 	processYUV_Altivec(imageStruct &image); 

    	//////////
    	// Set the new offset
    	void	    	vecOffsetMess(int argc, t_atom *argv);
    	
    	//////////
    	// Set the new offset
    	void	    	floatOffsetMess(float foffset);
    	
    	//////////
    	// The new offset
    	unsigned char 	m_color1[3],m_color2[3],m_thresh[3];
        
        short Y,U,V;

    private:
    
    	//////////
    	// Static member functions
    	static void 	color1MessCallback(void *data, t_floatarg value1, t_floatarg value2, t_floatarg value3);
    	static void 	color2MessCallback(void *data, t_floatarg value1, t_floatarg value2, t_floatarg value3);
        static void 	threshMessCallback(void *data, t_floatarg value1, t_floatarg value2, t_floatarg value3);
};

#endif	// for header file
