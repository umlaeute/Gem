/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    Change pix to greyscale with respect to alpha

    Copyright (c) 1997-2000 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    Copyright (c) 2002 James Tittle & Chris Clepper
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

/////////////////////////////////////////////////////////
//
//  pix_a_2grey
//
//  2002:forum::für::umläute:2000
//  iohannes m zmoelnig
//  mailto:zmoelnig@iem.mhsg.ac.at
//
/////////////////////////////////////////////////////////


#ifndef INCLUDE_pix_a_2grey_H_
#define INCLUDE_pix_a_2grey_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_a_2grey
    
    Change pix to greyscale with respect to alpha

KEYWORDS
    pix
    
DESCRIPTION
   
-----------------------------------------------------------------*/
class GEM_EXTERN pix_a_2grey : public GemPixObj
{
    CPPEXTERN_HEADER(pix_a_2grey, GemPixObj);

    public:

    //////////
    // Constructor
    pix_a_2grey(t_floatarg alpha);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_a_2grey();

    	//////////
    	// Do the processing
    	virtual void 	processRGBAImage(imageStruct &image);
        
	//////////
    	// Do the processing - this is a no-op
    	virtual void 	processGrayImage(imageStruct &image) { }

	//////////
	// alpha influences process
	// == 0 no influence
	// <  0 change pixes with alpha < -mode
	// >  0 change pixes with alpha >  mode
	int				m_mode;

    	//////////
    	// alpha setting
    	void	    	alphaMess(float alphaval);
    	
    private:
    	
    	//////////
    	// static member functions
    	static void 	alphaMessCallback(void *data, t_floatarg alphaval);
};

#endif	// for header file
